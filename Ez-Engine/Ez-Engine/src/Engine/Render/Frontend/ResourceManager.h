#ifndef RESOURCE_CLASS_MANAGER_H
#define RESOURCE_CLASS_MANAGER_H

#include<vector>
#include<string>
#include<unordered_map>

#include"Engine/Render/Frontend/Resources.h"
#include"Engine/Core/Logger.h"

namespace ENGINE {
	namespace RENDERER {

		class ResourceManager {
		public:
			void SetDevice(ID3D11Device* device) { this->device = device; }
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDepthStencilBuffer(int width, int height);

			template<typename T>
			std::shared_ptr<T> Load(std::shared_ptr<ASSET::Asset> asset);

		private:
			std::unordered_map<uint16_t, std::shared_ptr<Resource>> resources;

			ID3D11Device* device;
		};

		template<typename T>
		std::shared_ptr<T> ResourceManager::Load(std::shared_ptr<ASSET::Asset> asset) {
			static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource.");

			if (asset == nullptr) return nullptr;

			if (resources.contains(asset->id)) {
				return std::static_pointer_cast<T>(resources[asset->id]);
			}

			std::shared_ptr<T> resource = std::make_shared<T>();
			if (!((std::shared_ptr<Resource>)resource)->LoadFromAsset(asset, device)) {
				//LOG_WARN("[Resource Manager] Failed to load resource at path: " + filePath);
				return nullptr;
			}

			resources[asset->id] = resource;

			return resource;
		}

	}
}

#endif 