#include"Engine/Assets/AssetLoader.h"

// Mesh Loading
#include <iostream>
#include <fstream>
#include <sstream>

// Texture
#include"stb_image.h"


#include"Engine/Core/Logger.h"

namespace ENGINE {
	RawMesh MeshLoader::Load(const std::string& filepath) {
		RawMesh mesh;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            LOG_WARN("Failed to load mesh at path: " + filepath);
            return {};
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

                        mesh.vertices.push_back(vert);
                        mesh.indicies.push_back((uint64_t)mesh.vertices.size() - 1);
                    }
                }
            }
        }

        file.close();

        mesh.indexCount = static_cast<uint64_t>(mesh.indicies.size());

        return mesh;
	}

    RawTexture TextureLoader::Load(const std::string& filepath) {
        RawTexture raw;
        //stbi_set_flip_vertically_on_load(true);
        //raw.data = stbi_load(filepath.c_str(), &raw.width, &raw.height, &raw.channels, 4);

        return raw;
    }
}