#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include<vector>
#include<string>
#include<iostream>

#include"Engine/Core/Logger.h"
#include"Engine/Assets/Assets.h"

namespace ENGINE {
	namespace ASSET {

		class AssetManager {
		public:
			AssetManager() = default;

			bool HasAsset(std::string path, size_t& index);
			bool HasAsset(uint16_t id, size_t& index);
			uint16_t GetIdFromName(std::string name);
			bool LoadAsset(std::string path);

			std::shared_ptr<Asset> GetAsset(uint16_t id);
			AssetType GetAssetType(uint16_t id);

			template <typename T>
			std::shared_ptr<T> GetAssetAs(uint16_t id) {
				size_t hasIndex = 0;
				if (HasAsset(id, hasIndex)) {
					return std::static_pointer_cast<T>(m_loadedAssets[hasIndex]);
				}

				return nullptr;
			}

			std::vector<std::shared_ptr<Asset>>& GetLoadedAssets() { return m_loadedAssets; }

		private:
			std::vector<std::shared_ptr<Asset>> m_loadedAssets;
		};
	}
}

#endif