#include"Resources.h"

namespace ENGINE {
	namespace RENDERER {

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

        std::string GetErrorMessageFromBlob(ID3DBlob* errorBlob) {
            std::string errorMsg;
            if (errorBlob != nullptr) {
                errorMsg = std::string((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            return errorMsg;
        }

        bool Shader::LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) {
            if (asset->GetType() != ASSET::SHADER) {
                LOG_WARN("Wrong type of asset for shader resource.");
                return false;
            }

            std::shared_ptr<ASSET::Shader> shaderAsset = std::static_pointer_cast<ASSET::Shader>(asset);

            ID3DBlob* vsBlob = shaderAsset->vsBlob;
            ID3DBlob* psBlob = shaderAsset->psBlob;

            HRESULT hr;
            //CHECK_DXHR(hr, (std::string("Failed to create vertex shader. SHADER: ") + filePath).c_str());
            if (FAILED(hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader))) {
                return false;
            }

            //CHECK_DXHR(hr, (std::string("Failed to create pixel shader. SHADER: ") + filePath).c_str());
            if (FAILED(hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader))) {
                return false;
            }


            // Input layout
            D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };

            //CHECK_DXHR(hr, "Failed to create input layout");
            if (FAILED(hr = device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout)))
                return false;

            if (vsBlob) vsBlob->Release();
            if (psBlob) psBlob->Release();

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

        bool Mesh::LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) {
            if (asset->GetType() != ASSET::MESH) {
                LOG_WARN("Wrong type of asset for mesh resource.");
                return false;
            }

            HRESULT hr;

            std::shared_ptr<ASSET::Mesh> meshAsset = std::static_pointer_cast<ASSET::Mesh>(asset);

            std::vector<Vertex>& vertices = meshAsset->GetVertices();
            std::vector<uint64_t>& indices = meshAsset->GetIndicies();
            indexCount = meshAsset->GetIndexCount();

            // Vertex buffer
            D3D11_BUFFER_DESC vbDesc = {};
            vbDesc.Usage = D3D11_USAGE_DEFAULT;
            vbDesc.ByteWidth = sizeof(Vertex) * vertices.size();
            vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vbDesc.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = vertices.data();

            if (FAILED(hr = device->CreateBuffer(&vbDesc, &initData, &vertexBuffer))) return false;
            //CHECK_DXHR(hr, "Failed to create vertex buffer");

            // Index buffer
            D3D11_BUFFER_DESC ibDesc = {};
            ibDesc.Usage = D3D11_USAGE_DEFAULT;
            ibDesc.ByteWidth = sizeof(uint64_t) * indices.size();
            ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            ibDesc.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA iinitData = {};
            iinitData.pSysMem = indices.data();

            if (FAILED(hr = device->CreateBuffer(&ibDesc, &iinitData, &indexBuffer))) return false;
            //CHECK_DXHR(hr, "Failed to create index buffer");

            return true;
        }
        Texture::~Texture() {
            if (m_textureSRV) m_textureSRV.ReleaseAndGetAddressOf();
        }

        bool Texture::LoadFromAsset(std::shared_ptr<ASSET::Asset> asset, ID3D11Device* device) {
            if (asset->GetType() != ASSET::TEXTURE) {
                LOG_WARN("Wrong type of asset for shader resource.");
                return false;
            }

            ASSET::Texture* tex = static_cast<ASSET::Texture*>(asset.get());

            D3D11_TEXTURE2D_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.Width = tex->raw.width;
            desc.Height = tex->raw.height;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            D3D11_SUBRESOURCE_DATA initData;
            initData.pSysMem = tex->raw.data;
            initData.SysMemPitch = tex->raw.width * 4;
            initData.SysMemSlicePitch = 0;

            ID3D11Texture2D* texture = nullptr;
            
            HRESULT hr;

            if (FAILED(hr = device->CreateTexture2D(&desc, &initData, &texture))) return false;
            if (FAILED(hr = device->CreateShaderResourceView(texture, nullptr, &m_textureSRV))) return false;

            texture->Release();

            return true;
        }

	}
}