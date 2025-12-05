#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>
#include<string>

#include"Engine/ECS/Registry.h"

namespace ENGINE {
	class Scene {
	public:
		Scene(std::string sceneName) : sceneName(sceneName) {}

		void SetRegistry(ECS::Registry* registry) { this->registry = registry; }

		virtual void Setup() = 0;

	protected:
		ECS::Registry* registry;

	private:
		std::string sceneName = "";
	};
}

#endif