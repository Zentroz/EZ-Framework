#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<vector>
#include<d3d11.h>

#include"Math.h"
#include"ErrorHandler.h"

class Mesh {
public:
	Mesh(const char* path, const char* name);
	Mesh(std::vector<float3> vertices, std::vector<uint16_t> indices);
	~Mesh() noexcept;

	void CreateBuffers(ID3D11Device* device);
	void Bind(ID3D11DeviceContext* context);

private:
	struct Vertex {
		float3 position;
		//float3 normal;
		//float2 uv;
	};

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
};

#endif