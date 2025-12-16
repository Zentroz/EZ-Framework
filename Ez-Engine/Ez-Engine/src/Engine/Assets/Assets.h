#ifndef ASSETS_H
#define ASSETS_H

#include<vector>
#include<string>

// Shader
#include<d3d11.h>
#include<d3dcompiler.h>
#include<d3d11shader.h>
#include<wrl/client.h>

#include"Engine/Core/Math.h"
#include"Engine/Core/Logger.h"
#include"Engine/Utils/EUID.h"
#include"Engine/Serialization/MetaFile.h"
#include"Engine/Assets/RawAssets.h"

using namespace MATH;
									

namespace ENGINE {
	namespace ASSET {

		enum AssetType { TEXTURE, SHADER, MESH, SCRIPT, UNKNOWN };


		class Asset {
		public:
			EUID euid;
			std::string assetPath;

			Asset() = default;
			virtual ~Asset() = default;

			bool IsLoaded() { return isLoaded; }

			virtual void ApplyMetaData(MetaData* meta) {};

			virtual AssetType GetType() = 0;
			virtual bool Load() {
				isLoaded = true;
				return true;
			}

		private:
			bool isLoaded = false;
		};

		class Mesh : public Asset {
		public:
			bool readWrite;
			float scaleFactor = 1.0f;

		public:
			Mesh() = default;
			~Mesh() = default;

			void ApplyMetaData(MetaData* meta) override;

			AssetType GetType() override { return AssetType::MESH; }
			bool Load() override;

			std::vector<Vertex>& GetVertices() { return raw.vertices; };
			std::vector<uint64_t>& GetIndicies() { return raw.indicies; };
			uint64_t GetIndexCount() { return raw.indexCount; }

		private:
			RawMesh raw;
		};

		class Shader : public Asset {
		public:
			ID3DBlob* vsBlob = nullptr;
			ID3DBlob* psBlob = nullptr;
		public:
			Shader() = default;
			~Shader();

			AssetType GetType() override { return AssetType::SHADER; }
			bool Load() override;
		};

		class Texture : public Asset {
		public:
			TextureWrapMode wrapMode;
			TextureFilterMode filterMode;
			RawTexture raw;
		public:
			Texture() = default;

			void ApplyMetaData(MetaData* meta) override;

			AssetType GetType() override { return AssetType::TEXTURE; }
			bool Load() override;
		};
	}
}

#endif