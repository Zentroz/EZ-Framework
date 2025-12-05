#include"EntityManager.h"

namespace ENGINE {
	namespace ECS {

		EntityManager::EntityManager() : entitiesAlive(0) {}

		Entity EntityManager::CreateEntity(const char* name) {
			if (entitiesAlive >= MAX_ENTITIES) {
				EXCEPTION("Maximum number of entities reached.");
				return 0;
			}

			for (size_t i = 0; i < MAX_ENTITIES; i++)
			{
				if (!m_entities.test(i)) {
					m_entities.set(i);

					std::string eName = name == nullptr ? "Entity - " + std::to_string(i) : name;

					m_entitiesData[i] = GameEntity(i, eName);

					entitiesAlive++;
					return i;
				}
			}

			return 0;
		}

		void EntityManager::DestroyEntity(Entity entity) {
			if (!m_entities.test(entity)) {
				EXCEPTION("Trying to destroy an entity that does not exist.");
				return;
			}

			m_entitiesData.erase(entity);
			m_entities.reset(entity);
			entitiesAlive--;
		}

		bool EntityManager::IsEntityAlive(Entity entity) const {
			return m_entities.test(entity);
		}

	}
}