#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include<vector>
#include<string>
#include<iostream>

#include"Engine/Utils/EUID.h"
#include"Engine/Core/Logger.h"
#include"Engine/Assets/Assets.h"
#include"Engine/Assets/MetaFileCache.h"

namespace ENGINE {
	namespace ASSET {

		class AssetManager {
		public:
			AssetManager();

			bool HasAsset(EUID id, size_t& index);

			std::shared_ptr<Asset> GetAsset(EUID id);
			AssetType GetAssetType(EUID id);

			template <typename T>
			std::shared_ptr<T> GetAssetAs(EUID id) {
				size_t hasIndex = 0;
				if (HasAsset(id, hasIndex)) {
					return std::static_pointer_cast<T>(m_loadedAssets[hasIndex]);
				}

				return nullptr;
			}

			bool Load(const std::filesystem::path& filepath);

			MetaFileCache& GetMetaCache() { return loadedMetaFiles; }
			std::vector<std::shared_ptr<Asset>>& GetLoadedAssets() { return m_loadedAssets; }

		private:
			bool LoadFromMeta(EUID euid);

			std::vector<std::shared_ptr<Asset>> m_loadedAssets;
			MetaFileCache loadedMetaFiles;
		};
	}
}

#endif