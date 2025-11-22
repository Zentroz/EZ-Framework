#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Core/Math.h"
#include"Core/GameTime.h"

#include"Camera.h"
#include"RenderContext.h"
#include"RenderTarget.h"
#include"ResourceManager.h"
#include"RenderItem.h"

class Renderer {
public:
	Renderer();

	void Init(ID3D11Device* device, RenderContext* ctx, ID3D11DepthStencilState* defaultDepthState, ID3D11RasterizerState* defaultRasteriser);
	void Shutdown();
	void CreateGlobalBuffer(ID3D11Device* device);
	void SetGlobalBuffers();
	void InitRender(RenderTarget* mainRenderTarget);
	void Render(const std::vector<RenderItem>& renderList);
	void EndRender();
	
	Camera* GetCamera() noexcept { return &camera; }
private:
	RenderContext* ctx = nullptr;
	Camera camera;
	ResourceManager resources;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	ID3D11DepthStencilState* defaultDepthState = nullptr;
	ID3D11RasterizerState* defaultRasteriser = nullptr;

	struct PerObjectBuffer {
		XMMATRIX  model;
		float4 color;
	};

	ID3D11Buffer* perObjectBuffer = nullptr;
	// temp
	struct GlobalConstantBuffer {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		float4 lightDirection;
		float4 cameraPosition;
		float4 timeData;
	};

	ID3D11Buffer* globalBuffer = nullptr;
};

#endif