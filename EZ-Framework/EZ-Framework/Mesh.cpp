#include"Mesh.h"

Mesh::Mesh(const char* path, const char* name) {
    LoadMeshFromFile(path);

    /*
    int latitudeBands = 4;
    int longitudeBands = 4;
    float radius = 1.0f;

    std::vector<float3> vertices;
    std::vector<float3> normals;

    for (int lat = 0; lat <= latitudeBands; ++lat) {
        float theta = lat * 3.14 / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeBands; ++lon) {
            float phi = lon * 2.0f * 3.14 / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = sinTheta * cosPhi;
            float y = cosTheta;
            float z = sinTheta * sinPhi;

            normals.push_back(float3(x, y, z));
            vertices.push_back(float3(radius * x, radius * y, radius * z));
        }
    }

    for (int lat = 0; lat < latitudeBands; ++lat) {
        for (int lon = 0; lon < longitudeBands; ++lon) {
            int i1 = lat * (longitudeBands + 1) + lon;
            int i2 = i1 + longitudeBands + 1;
            int i3 = i2 + 1;
            int i4 = i1 + 1;

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i4);

            indices.push_back(i2);
            indices.push_back(i3);
            indices.push_back(i4);
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        Vertex v = { vertices[i], normals[i] };

        this->vertices.push_back(v);
    }
    */
}

Mesh::Mesh(std::vector<float3> vertices, std::vector<float3> normals, std::vector<uint16_t> indices)
	: indices(indices) {

    for (UINT i = 0; i < vertices.size(); i++ ) {
        this->vertices.push_back({ vertices[i], normals[i] });
    }
}

Mesh::Mesh(std::vector<Vertex> vertex, std::vector<uint16_t> indices) {
    this->vertices = vertex;
    this->indices = indices;
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

void Mesh::LoadMeshFromFile(std::string fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        EXCEPTION(("Failed to open mesh file" + fileName).c_str());
    }

    std::string line;

    std::vector<float3> positions;
    std::vector<float3> normals;
    std::vector<float2> texcoords;

    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    int normalCount = 0;
    int uvCount = 0;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "vt") {
            float2 uv;
            ss >> uv.x >> uv.y;
            texcoords.push_back(uv);
        }
        else if (prefix == "vn") {
            float3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f") {
            std::vector<std::string> vtns;
            std::string vtn;
            while (ss >> vtn) {
                vtns.push_back(vtn);
            }

            // triangulate (fan method)
            for (size_t i = 1; i + 1 < vtns.size(); i++) {
                std::string tri[] = { vtns[0], vtns[i], vtns[i + 1] };
                //std::string tri[] = { vtns[0], vtns[i + 1], vtns[i] };
                for (const std::string& token : tri) {
                    std::istringstream vs(token);
                    std::string vStr, tStr, nStr;
                    std::getline(vs, vStr, '/');
                    std::getline(vs, tStr, '/');
                    std::getline(vs, nStr, '/');

                    int vIndex = std::stoi(vStr) - 1;
                    int tIndex = tStr.empty() ? -1 : std::stoi(tStr) - 1;
                    int nIndex = nStr.empty() ? -1 : std::stoi(nStr) - 1;

                    Vertex vert{};
                    vert.position = positions[vIndex];
                    if (nIndex >= 0) vert.normal = normals[nIndex];
                    //if (tIndex >= 0) vert.uv = texcoords[tIndex];

                    vertices.push_back(vert);
                    indices.push_back((UINT)vertices.size() - 1);
                }
            }
        }

        /*else if (prefix == "f") {
            std::string vtn[3];
            ss >> vtn[0] >> vtn[1] >> vtn[2];

            for (int i = 0; i < 3; i++) {
                std::istringstream vs(vtn[i]);
                std::string vStr, tStr, nStr;

                std::getline(vs, vStr, '/');
                std::getline(vs, tStr, '/');
                std::getline(vs, nStr, '/');

                int vIndex = std::stoi(vStr) - 1;
                int tIndex = tStr.empty() ? -1 : std::stoi(tStr) - 1;
                int nIndex = nStr.empty() ? -1 : std::stoi(nStr) - 1;

                Vertex vert{};
                vert.position = positions[vIndex];

                if (nIndex >= 0) {
                    vert.normal = normals[nIndex];
                }
                if (tIndex >= 0) {
                    vert.uv = texcoords[tIndex];
                }

                vertices.push_back(vert);
                indices.push_back((UINT)vertices.size() - 1);
            }
        }*/
    }

    for (const Vertex& vertex : vertices) {
        this->vertices.push_back(vertex);
    }

    for (UINT index : indices) {
        this->indices.push_back(index);
    }

    file.close();
}