#include"Engine/Render/Scene.h"

namespace ENGINE {

	EUID Scene::CreateEntity(std::string name) {
		EUID e = p_Registry->CreateEntity(name);
		m_Entities.push_back(e);
		return e;
	}

	void Scene::DestroyEntity(EUID entity) {
		p_Registry->DestroyEntity(entity);

		for (int i = 0; i < m_Entities.size(); i++) {
			EUID e = m_Entities[i];
			if (e == entity) m_Entities.erase(m_Entities.begin() + i);
		}
	}

}