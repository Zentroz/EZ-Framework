#ifndef ASSETS_H
#define ASSETS_H

#include<vector>
#include<string>

// Shader
#include<d3d11.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include<wrl/client.h>

// Mesh
#include <iostream>
#include <fstream>
#include <sstream>

#include"Engine/Core/Math.h"
#include"Engine/Core/Logger.h"

using namespace MATH;

namespace ENGINE {
	namespace ASSET {

		enum AssetType { TEXTURE, SHADER, MESH, SCRIPT, UNKNOWN };

		class Asset {
		public:
			uint16_t id;
			std::string path;
			std::string name;

			Asset() = default;
			virtual ~Asset() = default;
			virtual bool LoadFromFile(std::string path) {
				this->path = path;

				size_t nameLength = 0;

				for (size_t i = path.length() - 1; i >= 0; i--) {
					nameLength++;
					char ch = path.at(i);
					if (ch == '/' || ch == '\\') {
						name = path.substr(i + 1, nameLength - 1);
						break;
					}
				}

				return true;
			}

			virtual AssetType GetType() = 0;
		};

		class Mesh : public Asset {
		public:
			struct Vertex {
				float3 position;
				float3 normal;
				float2 uv;
			};

			Mesh() = default;
			~Mesh();
			bool LoadFromFile(std::string path) override;
			AssetType GetType() override { return AssetType::MESH; }

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			UINT indexCount;
		};

		class Shader : public Asset {
		public:
			Shader() = default;
			~Shader();
			bool LoadFromFile(std::string path) override;
			AssetType GetType() override { return AssetType::SHADER; }

			ID3DBlob* vsBlob = nullptr;
			ID3DBlob* psBlob = nullptr;
		};

		class Texture : public Asset {
		public:
			Texture() = default;
			bool LoadFromFile(std::string path) override;
			AssetType GetType() override { return AssetType::TEXTURE; }
		};
	}
}

#endif