#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include<bitset>
#include<unordered_map>

#include"Engine/Core/ErrorHandler.h"
#include"Engine/ECS/ECSData.h"

namespace ENGINE {
	namespace ECS {

		struct GameEntity {
			uint32_t id;
			std::string name;

			GameEntity() = default;
			GameEntity(uint32_t id, std::string name) : id(id), name(name) {}
		};

		class EntityManager {
		public:
			EntityManager();

			Entity CreateEntity(const char* name = nullptr);
			void DestroyEntity(Entity entity);

			bool IsEntityAlive(Entity entity) const;

			GameEntity* GetGameEntity(Entity entity) {
				if (!IsEntityAlive(entity)) return nullptr;
				return &m_entitiesData[entity];
			}

		private:
			std::bitset<MAX_ENTITIES> m_entities{};
			std::unordered_map<Entity, GameEntity> m_entitiesData{};
			unsigned int entitiesAlive;
		};

	}
}

#endif