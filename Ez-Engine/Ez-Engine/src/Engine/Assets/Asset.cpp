#include"Assets.h"

#include"Engine/Assets/AssetLoader.h"
#include"Engine/Serialization/BinarySerializer.h"

namespace ENGINE {
	namespace ASSET {

        std::string GetErrorMessageFromBlob(ID3DBlob* errorBlob) {
            std::string errorMsg;
            if (errorBlob != nullptr) {
                errorMsg = std::string((char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }

            return errorMsg;
        }

        Shader::~Shader() {}

		bool Shader::Load() {
            std::wstring wfile(assetPath.begin(), assetPath.end());
            HRESULT hr;
            ID3DBlob* errBlob = nullptr;

            hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob);
            //CHECK_SHADER(hr, errBlob, filePath.c_str());
            if (FAILED(hr)) {
                LOG_WARN("Failed to load shader file: " + GetErrorMessageFromBlob(errBlob));
                if (errBlob) errBlob->Release();
                return false;
            }

            hr = D3DCompileFromFile(wfile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob);
            //CHECK_SHADER(hr, errBlob, filePath.c_str());
            if (FAILED(hr)) {
                LOG_WARN("Failed to load shader file: " + GetErrorMessageFromBlob(errBlob));
                if (errBlob) errBlob->Release();
                return false;
            }

            if (errBlob) errBlob->Release();

            Asset::Load();

            return true;
		}

        void Mesh::ApplyMetaData(MetaData* meta) {
            if (meta->GetType() != MetaMesh) return;
            MeshMetaData* mesh = static_cast<MeshMetaData*>(meta);

            readWrite = mesh->readWrite;
            scaleFactor = mesh->scaleFactor;
        }

		bool Mesh::Load() {
            BinarySerializer::ReadMesh(assetPath, raw);

            if (raw.vertices.size() == 0 || raw.indexCount == 0) return false;

            Asset::Load();
            return true;
		}

        void Texture::ApplyMetaData(MetaData* meta) {
            if (meta->GetType() != MetaTexture) return;
            assetPath = meta->assetPath;

            TextureMetaData* tex = static_cast<TextureMetaData*>(meta);

            filterMode = tex->filterMode;
            wrapMode = tex->wrapMode;
        }

        bool Texture::Load() {
            BinarySerializer::ReadTexture(assetPath, raw);

            Asset::Load();
            return true;
        }

	}
}