#include"Engine/RenderContext.h"

RenderContext::RenderContext(ID3D11DeviceContext* ctx) : ctx(ctx) {}

void RenderContext::ClearRenderTarget(ID3D11RenderTargetView* renderTarget, float4 color) {
	float clearColor[4] = { color.x, color.y, color.z, color.w };
	ctx->ClearRenderTargetView(renderTarget, clearColor);
}

void RenderContext::SetRenderTarget(ID3D11RenderTargetView* renderTarget) {
	ctx->OMSetRenderTargets(1, &renderTarget, nullptr);
}

void RenderContext::SetTopology() {
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RenderContext::SetShader(Shader* shader) {
	ctx->VSSetShader(shader->GetVertexShader(), nullptr, 0);
	ctx->PSSetShader(shader->GetPixelShader(), nullptr, 0);
}

void RenderContext::DrawMesh(Mesh* mesh) {
	UINT stride = mesh->GetStride();
	UINT offset = mesh->GetStride();

	ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();

	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	ctx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
}

void RenderContext::SetVSConstantBuffer(ID3D11Buffer* buffer, UINT slot) {
	ctx->VSSetConstantBuffers(slot, 1, &buffer);
}

void RenderContext::SetPSConstantBuffer(ID3D11Buffer* buffer, UINT slot) {
	ctx->PSSetConstantBuffers(slot, 1, &buffer);
}

void RenderContext::SetConstantBuffer(ID3D11Buffer* buffer, const void* data) {
	ctx->UpdateSubresource(buffer, 0, nullptr, &data, 0, 0);
}

void RenderContext::DrawIndexed(uint32_t indexCount) {
	ctx->DrawIndexed(indexCount, 0, 0);
}