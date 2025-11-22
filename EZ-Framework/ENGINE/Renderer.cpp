#include"Engine/Renderer.h"

Renderer::Renderer() {}

void Renderer::Init(ID3D11Device* device, RenderContext* ctx, ID3D11DepthStencilState* defaultDepthState, ID3D11RasterizerState* defaultRasteriser) {
	this->ctx = ctx;
	this->defaultDepthState = defaultDepthState;
	this->defaultRasteriser = defaultRasteriser;

	resources.SetDevice(device);

	depthStencilView = resources.CreateDepthStencilBuffer();

	CreateGlobalBuffer(device);
}

void Renderer::Shutdown() {}

void Renderer::CreateGlobalBuffer(ID3D11Device* device) {
	HRESULT hr;

	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(GlobalConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&cbd, nullptr, &globalBuffer);

	CHECK_DXHR(hr, "Failed to create global buffer.");

	D3D11_BUFFER_DESC pOD = {};
	pOD.Usage = D3D11_USAGE_DYNAMIC;
	pOD.ByteWidth = sizeof(PerObjectBuffer);
	pOD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pOD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&pOD, nullptr, &perObjectBuffer);

	CHECK_DXHR(hr, "Failed to create Per Object buffer.");
}

void Renderer::SetGlobalBuffers() {
	GlobalConstantBuffer cbData;

	cbData.view = XMMatrixTranspose(camera.GetViewMatrix());
	cbData.projection = XMMatrixTranspose(camera.GetProjectionMatrix(1.0f));
	float3 camPos = camera.position;
	cbData.cameraPosition = float4(camPos.x, camPos.y, camPos.z, 0);
	cbData.lightDirection = (quaternion::FromAxisAngle(float3(0, 1, 0), 30 * Deg2Rad) * quaternion::FromAxisAngle(float3(1, 0, 0), 65 * Deg2Rad)).forward() * -1;
	cbData.timeData = float4(GameTime::time, GameTime::deltaTime, 0, 0);

	ctx->UpdateMappedSubresource(globalBuffer, &cbData, sizeof(cbData));
}

void Renderer::InitRender(RenderTarget* mainRenderTarget) {
	ctx->ClearRenderTarget(mainRenderTarget->GetRTV(), float4(0.15f, 0.15f, 0.3f, 1.0f));
	ctx->ClearDepthStencilView(depthStencilView.Get());

	ctx->SetRenderTarget(mainRenderTarget->GetRTV(), depthStencilView.Get());
	ctx->SetDepthStencilState(defaultDepthState);
	ctx->SetViewport();

	ctx->SetRasteriser(defaultRasteriser);
}

void Renderer::Render(const std::vector<RenderItem>& renderList) {
	SetGlobalBuffers();

	PerObjectBuffer perObjectData;

	ctx->SetTopology();

	for (const RenderItem& item : renderList) {
		std::shared_ptr<Shader> shader = resources.Load<Shader>(item.material.shaderPath);
		std::shared_ptr<Mesh> mesh = resources.Load<Mesh>(item.mesh.path);
		std::shared_ptr<Texture> texture = resources.Load<Texture>(item.material.mainTexturePath);

		perObjectData.model = item.model;
		perObjectData.color = item.material.baseColor;

		ctx->UpdateMappedSubresource(perObjectBuffer, &perObjectData, sizeof(perObjectData));

		ctx->SetInputLayout(shader->GetInputLayout());

		ctx->SetShader(shader.get());

		ID3D11ShaderResourceView* textureSRV = nullptr;

		if (texture != nullptr) {
			textureSRV = texture->GetSRV();
			ctx->SetSamplerState(0, texture->GetSamplerAddress());
		}

		ctx->SetPSShaderResource(0, &textureSRV);

		ctx->SetVSConstantBuffer(perObjectBuffer, 1);
		ctx->SetPSConstantBuffer(perObjectBuffer, 1);
		ctx->SetVSConstantBuffer(globalBuffer, 0);
		ctx->SetPSConstantBuffer(globalBuffer, 0);

		ctx->DrawMesh(mesh.get());

		ctx->DrawIndexed(mesh->GetIndexCount());
	}
}

void Renderer::EndRender() {
	ctx->SetRenderTarget(nullptr, nullptr);
}