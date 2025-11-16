#ifndef RESOURCE_CLASS_MANAGER_H
#define RESOURCE_CLASS_MANAGER_H

#include<vector>
#include<string>

#include"Engine/Shader.h"
#include"Engine/Material.h"

class ResourceManager {
public:
	//void LoadResources(ID3D11Device* device);

private:
	std::vector<Shader> shaders = {};
	std::vector<Material> materials = {};

private:
	class Resource {
	public:
		std::string filePath;
		virtual void LoadFromFile(std::string filePath) = 0;
	};
};

#endif 