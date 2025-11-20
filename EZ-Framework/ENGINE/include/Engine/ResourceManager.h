#ifndef RESOURCE_CLASS_MANAGER_H
#define RESOURCE_CLASS_MANAGER_H

#include<vector>
#include<string>
#include<unordered_map>

#include"Engine/Resources.h"

class ResourceManager {
public:
	void SetDevice(ID3D11Device* device) { this->device = device; }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> CreateDepthStencilBuffer();

	template<typename T>
	std::shared_ptr<T> Load(std::string filePath);

private:
	std::unordered_map<std::string, std::shared_ptr<Resource>> resources;

	ID3D11Device* device;
};

template<typename T>
std::shared_ptr<T> ResourceManager::Load(std::string filePath) {
	static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource.");

	if (resources.contains(filePath)) {
		return std::static_pointer_cast<T>(resources[filePath]);
	}

	std::shared_ptr<T> resource = std::make_shared<T>();
	if (!resource->LoadFromFile(filePath, device)) {
		return nullptr;
	}

	resources[filePath] = resource;

	return resource;
}

#endif 