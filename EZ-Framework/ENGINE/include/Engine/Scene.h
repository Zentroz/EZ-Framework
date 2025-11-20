#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>
#include<string>

#include"Engine/Registry.h"

class Scene {
public:
	Scene(std::string sceneName);

	void SetRegistry(Registry* registry) { this->registry = registry; }

	virtual void Setup() = 0;

protected:
	Registry* registry;

private:
	std::string sceneName = "";
};

#endif