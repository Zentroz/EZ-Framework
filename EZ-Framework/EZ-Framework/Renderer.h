#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Graphics.h"
#include"Mesh.h"
#include"Material.h"
#include"Camera.h"
#include"Time.h"

class Renderer {
public:
	Renderer(HWND hWnd);

	void CreateGlobalBuffer();
	void SetGlobalBuffers();
	void Render();
	
	Camera& GetCamera() noexcept { return camera; }
private:
	Graphics graphics;
	Mesh testMesh;
	Shader testShader;
	Material testMaterial;
	Camera camera;

	// temp
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

	float3 testMeshPosition;
	float3 lightPosition;
};

#endif