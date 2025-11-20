#include"Engine/Resources.h"

Shader::~Shader() {
	if (vertexShader) {
		vertexShader->Release();
		vertexShader = nullptr;
	}
	if (pixelShader) {
		pixelShader->Release();
		pixelShader = nullptr;
	}
}

bool Shader::LoadFromFile(std::string filePath, ID3D11Device* device) {
	this->filePath = filePath;

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errBlob = nullptr;

	std::wstring wfile(filePath.begin(), filePath.end());

	HRESULT hr;

	hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, filePath.c_str());

	hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob);
	CHECK_SHADER(hr, errBlob, filePath.c_str());

	hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	CHECK_DXHR(hr, (std::string("Failed to create vertex shader. SHADER: ") + filePath).c_str());

	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
	CHECK_DXHR(hr, (std::string("Failed to create pixel shader. SHADER: ") + filePath).c_str());

    // Input layout
    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
    CHECK_DXHR(hr, "Failed to create input layout");

	if (FAILED(hr)) return false;

	if (vsBlob)vsBlob->Release();
	if (psBlob)psBlob->Release();
	if (errBlob)errBlob->Release();

	return true;
}

Mesh::~Mesh() {
	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}

bool Mesh::LoadFromFile(std::string filePath, ID3D11Device* device) {
    this->filePath = filePath;

    std::ifstream file(filePath);

    if (!file.is_open()) {
        EXCEPTION(("Failed to open mesh file" + filePath).c_str());
        return false;
    }

    std::string line;

    std::vector<float3> positions;
    std::vector<float3> normals;
    std::vector<float2> texcoords;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

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
                    if (tIndex >= 0) vert.uv = texcoords[tIndex];

                    vertices.push_back(vert);
                    indices.push_back((uint32_t)vertices.size() - 1);
                }
            }
        }
    }

    file.close();

    this->indexCount = static_cast<UINT>(indices.size());

    return CreateBuffers(device, vertices, indices);
}

bool Mesh::CreateBuffers(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
    HRESULT hr;

    // Vertex buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.ByteWidth = sizeof(Vertex) * vertices.size();
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();

    hr = device->CreateBuffer(&vbDesc, &initData, &vertexBuffer);
    CHECK_DXHR(hr, "Failed to create vertex buffer");

    // Index buffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.ByteWidth = sizeof(uint32_t) * indices.size();
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData = {};
    iinitData.pSysMem = indices.data();

    hr = device->CreateBuffer(&ibDesc, &iinitData, &indexBuffer);
    CHECK_DXHR(hr, "Failed to create index buffer");

    if (FAILED(hr)) return false;

    return true;
}