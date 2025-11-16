#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Core/Math.h"
#include"Core/Time.h"

#include"Graphics.h"
#include"Mesh.h"
#include"Material.h"
#include"Camera.h"

class Renderer {
public:
	Renderer();

	void Init(HWND hWnd);
	void Shutdown();
	void CreateGlobalBuffer();
	void SetGlobalBuffers();
	void Render();
	
	Camera& GetCamera() noexcept { return camera; }
private:
	Graphics graphics;
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