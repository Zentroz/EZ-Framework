#include"Mesh.h"

Mesh::Mesh(const char* path, const char* name) {}

Mesh::Mesh(std::vector<float3> vertices, std::vector<uint16_t> indices)
	: indices(indices) {

    for (const auto& v : vertices) {
        this->vertices.push_back({ v });
    }
}

Mesh::~Mesh() {
    if (m_pVertexBuffer) {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = nullptr;
    }
    if (m_pIndexBuffer) {
        m_pIndexBuffer->Release();
        m_pIndexBuffer = nullptr;
    }
}

void Mesh::CreateBuffers(ID3D11Device* device) {
    HRESULT hr;

    // Vertex buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(Vertex) * vertices.size();
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    hr = device->CreateBuffer(&vbDesc, &initData, &m_pVertexBuffer);
	CHECK_DXHR(hr, "Failed to create vertex buffer");

    // Index buffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(uint16_t) * indices.size();
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData = {};
    iinitData.pSysMem = indices.data();

    hr = device->CreateBuffer(&ibDesc, &iinitData, &m_pIndexBuffer);
	CHECK_DXHR(hr, "Failed to create index buffer");
}

void Mesh::Bind(ID3D11DeviceContext* context) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}