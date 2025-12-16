#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include<vector>
#include<string>

#include"Engine/ECS/Registry.h"

namespace ENGINE {
	class Scene {
	public:
		std::string sceneName;
	public:
		Scene(std::string sceneName) : sceneName(sceneName) {}

		void SetRegistry(ECS::Registry* registry) { this->p_Registry = registry; }
		ECS::Registry* GetRegistry() { return p_Registry; }

		EUID CreateEntity(std::string name);
		void DestroyEntity(EUID entity);

		const std::vector<EUID>& GetEntities() const { return m_Entities; }

	private:
		std::vector<EUID> m_Entities;
		ECS::Registry* p_Registry = nullptr;
	};
}

#endif