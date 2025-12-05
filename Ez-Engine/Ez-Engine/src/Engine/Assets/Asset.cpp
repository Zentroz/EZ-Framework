#include"Assets.h"

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

		bool Shader::LoadFromFile(std::string path) {
            Asset::LoadFromFile(path);

            std::wstring wfile(path.begin(), path.end());

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

            return true;
		}

        Mesh::~Mesh() {
            vertices.clear();
            indices.clear();
        }

		bool Mesh::LoadFromFile(std::string path) {
            Asset::LoadFromFile(path);

            std::ifstream file(path);

            if (!file.is_open()) {
                Logger::LogMessage(Log(LogType::Warning, "Failed to load mesh at path: " + path));
                return false;
            }

            std::string line;

            std::vector<float3> positions;
            std::vector<float3> normals;
            std::vector<float2> texcoords;

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

            return true;
		}

        bool Texture::LoadFromFile(std::string path) {
            Asset::LoadFromFile(path);
            return true;
        }

	}
}