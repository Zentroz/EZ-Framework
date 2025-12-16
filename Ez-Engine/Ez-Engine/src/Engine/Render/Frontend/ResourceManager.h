#ifndef RESOURCE_CLASS_MANAGER_H
#define RESOURCE_CLASS_MANAGER_H

#include<vector>
#include<string>
#include<unordered_map>

#include"Engine/Render/Frontend/Resources.h"
#include"Engine/Core/Logger.h"
#include"Engine/Assets/AssetManager.h"

namespace ENGINE {
	namespace RENDERER {

		class ResourceManager {
		public:
			void SetDevice(ID3D11Device* device) { this->device = device; }
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDepthStencilBuffer(int width, int height);

			void SetAssetManager(ASSET::AssetManager* assetManager) { this->assetManager = assetManager; };

			template<typename T>
			std::shared_ptr<T> Load(EUID euid);

		private:
			std::unordered_map<EUID, std::shared_ptr<Resource>> resources;
			ASSET::AssetManager* assetManager = nullptr;
			ID3D11Device* device;
		};

		template<typename T>
		std::shared_ptr<T> ResourceManager::Load(EUID euid) {
			static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource.");

			if (resources.contains(euid)) {
				return std::static_pointer_cast<T>(resources[euid]);
			}

			std::shared_ptr<ASSET::Asset> asset = assetManager->GetAsset(euid);
			if (asset == nullptr) return nullptr;

			std::shared_ptr<T> resource = std::make_shared<T>();
			if (!((std::shared_ptr<Resource>)resource)->LoadFromAsset(asset, device)) {
				//LOG_WARN("[Resource Manager] Failed to load resource at path: " + filePath);
				return nullptr;
			}

			resources[asset->euid] = resource;

			return resource;
		}

	}
}

#endif 