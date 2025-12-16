#include"AssetManager.h"
#include"Engine/Assets/AssetImporter.h"

namespace ENGINE {
	namespace ASSET {

		AssetManager::AssetManager() {}

		bool AssetManager::HasAsset(EUID id, size_t& index) {
			size_t i = 0;

			for (auto asset : m_loadedAssets) {
				if (asset->euid == id) {
					index = i;
					return true;
				}

				i++;
			}

			return false;
		}

		bool AssetManager::LoadFromMeta(EUID euid) {
			MetaData* meta = loadedMetaFiles.Get(euid);
			if (meta == nullptr) return false;

			std::shared_ptr<Asset> asset;

			MetaAssetType type = meta->GetType();

			switch (type) {
			case MetaAssetType::MetaMesh:
				asset = std::make_shared<Mesh>();
				break;
			case MetaAssetType::MetaTexture:
				asset = std::make_shared<Texture>();
				break;
			case MetaAssetType::MetaShader:
				asset = std::make_shared<Shader>();
				break;
			}

			asset->ApplyMetaData(meta);

			m_loadedAssets.push_back(asset);
			return true;
		}

		AssetType AssetManager::GetAssetType(EUID id) {
			std::shared_ptr<Asset> asset = GetAsset(id);
			if (asset == nullptr) return UNKNOWN;
			return asset->GetType();
		}

		std::shared_ptr<Asset> AssetManager::GetAsset(EUID id) {
			// 1. has asset - return / load
			size_t hasIndex = 0;

			if (HasAsset(id, hasIndex)) {
				return m_loadedAssets[hasIndex];
			}

			if (LoadFromMeta(id)) {
				HasAsset(id, hasIndex);
				return m_loadedAssets[hasIndex];
			}

			return nullptr;
		}

		bool AssetManager::Load(const fs::path& filepath) {
			if (filepath.extension() == ".obj") {
				MeshImporter importer;
				auto meta = importer.Import(filepath);

				loadedMetaFiles.Add(meta);
			}
			else if (filepath.extension() == ".png") {

			}
			else if (filepath.extension() == "hlsl") {

			}

			return true;
		}

	}
}