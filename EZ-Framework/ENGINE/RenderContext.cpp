#include"Engine/RenderContext.h"

RenderContext::RenderContext(ID3D11DeviceContext* ctx) : ctx(ctx) {}

void RenderContext::ClearRenderTarget(ID3D11RenderTargetView* renderTarget, float4 color) {
	float clearColor[4] = { color.x, color.y, color.z, color.w };
	ctx->ClearRenderTargetView(renderTarget, clearColor);
}

void RenderContext::ClearDepthStencilView(ID3D11DepthStencilView* view) {
	ctx->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderContext::SetShaderResource() {}

void RenderContext::SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthView) {
	ctx->OMSetRenderTargets(1, &renderTarget, depthView);
}

void RenderContext::SetDepthStencilState(ID3D11DepthStencilState* state) {
	ctx->OMSetDepthStencilState(state, 0);
}

void RenderContext::SetTopology() {
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RenderContext::SetInputLayout(ID3D11InputLayout* layout) {
	ctx->IASetInputLayout(layout);
}

void RenderContext::SetRasteriser(ID3D11RasterizerState* state) {
	ctx->RSSetState(state);
}

void RenderContext::SetViewport() {
	D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(600);
    viewport.Height = static_cast<FLOAT>(600);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

	ctx->RSSetViewports(1, &viewport);
}

void RenderContext::SetShader(Shader* shader) {
	ctx->VSSetShader(shader->GetVertexShader(), nullptr, 0);
	ctx->PSSetShader(shader->GetPixelShader(), nullptr, 0);
}

void RenderContext::DrawMesh(Mesh* mesh) {
	UINT stride = mesh->GetStride();
	UINT offset = mesh->GetOffset();

	ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
	ID3D11Buffer* iBuffer = mesh->GetIndexBuffer();

	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	ctx->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void RenderContext::SetVSConstantBuffer(ID3D11Buffer* buffer, UINT slot) {
	ctx->VSSetConstantBuffers(slot, 1, &buffer);
}

void RenderContext::SetPSConstantBuffer(ID3D11Buffer* buffer, UINT slot) {
	ctx->PSSetConstantBuffers(slot, 1, &buffer);
}

void RenderContext::UpdateMappedSubresource(ID3D11Buffer* buffer, void* data, size_t dataSize) {
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	memcpy(mapped.pData, data, dataSize);

	ctx->Unmap(buffer, 0);
}

void RenderContext::UpdateSubresource(ID3D11Buffer* buffer, const void* data) {
	ctx->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
}

void RenderContext::DrawIndexed(uint32_t indexCount) {
	ctx->DrawIndexed(indexCount, 0, 0);
}