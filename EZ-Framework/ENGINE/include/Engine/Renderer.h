#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Core/Math.h"
#include"Core/Time.h"

#include"Mesh.h"
#include"Material.h"
#include"Camera.h"
#include"RenderContext.h"
#include"RenderTarget.h"

class Renderer {
public:
	Renderer();

	void Init(ID3D11Device* device, RenderContext* ctx);
	void Shutdown();
	void CreateGlobalBuffer();
	void SetGlobalBuffers();
	void InitRender(RenderTarget* mainRenderTarget);
	void Render();
	void EndRender();
	
	Camera& GetCamera() noexcept { return camera; }
private:
	RenderContext* ctx = nullptr;
	Camera camera;

	// temp
	Mesh testMesh;
	Shader testShader;
	Material testMaterial;
	struct GlobalConstantBuffer {
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		float4 lightDirection;
		float4 cameraPosition;
		float time;
		float deltaTime;
		float2 padding;
	};

	ID3D11Buffer* globalBuffer = nullptr;
};

#endif