#include"Engine/Renderer.h"

Renderer::Renderer() {}

void Renderer::Init(ID3D11Device* device, RenderContext* ctx) {
	this->ctx = ctx;
}

void Renderer::Shutdown() {
	//graphics.Shutdown();
}

void Renderer::CreateGlobalBuffer() {
	HRESULT hr;

	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(GlobalConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	//hr = graphics.GetDevice()->CreateBuffer(&cbd, nullptr, &globalBuffer);
}

void Renderer::SetGlobalBuffers() {
	float3 lightPosition = float3(0, 0, -3);
	float3 testMeshPosition = float3(0, 0, 1);

	GlobalConstantBuffer cbData;

	cbData.model = XMMatrixTranspose(
		XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(sin(Time::time / 6) * 360)) *
		XMMatrixTranslation(testMeshPosition.x, testMeshPosition.y, testMeshPosition.z)
	);

	cbData.view = XMMatrixTranspose(camera.GetViewMatrix());
	cbData.projection = XMMatrixTranspose(camera.GetProjectionMatrix(1.0f));
	cbData.cameraPosition = camera.GetPosition();
	cbData.lightDirection = (lightPosition - testMeshPosition).normalize();
	cbData.deltaTime = Time::deltaTime;
	cbData.time = Time::time;

	ctx->SetConstantBuffer(globalBuffer, &cbData);
}

void Renderer::InitRender(RenderTarget* mainRenderTarget) {
	ctx->ClearRenderTarget(mainRenderTarget->GetRTV(), float4(0.15f, 0.15f, 0.3f, 1.0f));

	ctx->SetRenderTarget(mainRenderTarget->GetRTV());
}

void Renderer::Render() {
	// Draw calls here
}
void Renderer::EndRender() {
	ctx->SetRenderTarget(nullptr);
	//ctx->SetShaderResource(nullptr, 1);
}
