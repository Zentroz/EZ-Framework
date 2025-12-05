#include"AssetManager.h"

namespace ENGINE {
	namespace ASSET {
		bool AssetManager::HasAsset(std::string path, size_t& index) {
			size_t i = 0;

			for (auto asset : m_loadedAssets) {
				if (asset->path == path) {
					index = i;
					return true;
				}

				i++;
			}

			return false;
		}

		bool AssetManager::HasAsset(uint16_t id, size_t& index) {
			size_t i = 0;

			for (auto asset : m_loadedAssets) {
				if (asset->id == id) {
					index = i;
					return true;
				}

				i++;
			}

			return false;
		}
		uint16_t AssetManager::GetIdFromName(std::string name) {

			for (auto asset : m_loadedAssets) {
				if (asset->name == name) {
					return asset->id;
				}
			}

			return UINT16_MAX;
		}

		bool AssetManager::LoadAsset(std::string path) {
			if (path.empty()) {
				LOG_WARN("Path is empty.");
				return false;
			}

			std::string extension;
			size_t extLength = 0;

			for (size_t i = path.length() - 1; i >= 0; i--) {
				extLength++;
				char ch = path.at(i);
				if (ch == '.') {
					extension = path.substr(i, extLength);
					break;
				}
			}

			size_t hasIndex = 0;

			if (HasAsset(path, hasIndex)) {
				LOG_WARN("Already has the asset.");
				return true;
			}

			std::shared_ptr<Asset> asset;

			if (extension == ".hlsl") {
				asset = (std::shared_ptr<Asset>)std::make_shared<Shader>();
			}
			else if (extension == ".obj") {
				asset = (std::shared_ptr<Asset>)std::make_shared<Mesh>();
			}
			else if (extension == ".png") {
				asset = (std::shared_ptr<Asset>)std::make_shared<Texture>();
			}
			else {
				LOG_WARN("Extension is not supported.");
				return false;
			}

			if (!asset->LoadFromFile(path)) {
				LOG_WARN("Failed to load asset.");
				return false;
			}

			asset->id = m_loadedAssets.size();

			m_loadedAssets.push_back(asset);

			LOG_INFO("Asset loaded!");

			return true;
		}

		AssetType AssetManager::GetAssetType(uint16_t id) {
			std::shared_ptr<Asset> asset = GetAsset(id);
			if (asset == nullptr) return UNKNOWN;
			return asset->GetType();
		}

		std::shared_ptr<Asset> AssetManager::GetAsset(uint16_t id) {
			size_t hasIndex = 0;
			if (HasAsset(id, hasIndex)) {
				return m_loadedAssets[hasIndex];
			}

			return nullptr;
		}
	}
}