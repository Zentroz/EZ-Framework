#ifndef RESOURCE_CLASSES_H
#define RESOURCE_CLASSES_H

#include"Core/ErrorHandler.h"
#include"Core/Math.h"

#include<vector>

// Shader
#include<d3d11.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include<wrl/client.h>

// Mesh
#include <iostream>
#include <fstream>
#include <sstream>

class Resource {
public:
	std::string filePath;

	virtual ~Resource() = default;
	virtual bool LoadFromFile(std::string filePath, ID3D11Device* device) = 0;
};

class Shader : public Resource {
public:
	~Shader();
	bool LoadFromFile(std::string filePath, ID3D11Device* device) override;

	ID3D11VertexShader* GetVertexShader() const { return vertexShader; }
	ID3D11PixelShader* GetPixelShader() const { return pixelShader; }
	ID3D11InputLayout* GetInputLayout() const { return inputLayout; }

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
};

class Mesh : public Resource {
public:
	struct Vertex {
		float3 position;
		float3 normal;
		float2 uv;
	};

	~Mesh();
	bool LoadFromFile(std::string filePath, ID3D11Device* device) override;
	bool CreateBuffers(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; }

	UINT GetIndexCount() const { return indexCount; }
	UINT GetOffset() { return 0; }
	UINT GetStride() { return sizeof(Vertex); }
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT indexCount;
};

#endif