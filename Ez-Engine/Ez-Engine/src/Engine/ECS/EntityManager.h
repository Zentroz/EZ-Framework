#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include<unordered_map>
#include<bitset>
#include<optional>

#include"Engine/Core/Logger.h"
#include"Engine/Core/ErrorHandler.h"
#include"Engine/ECS/ECSData.h"
#include"Engine/Utils/EUID.h"
#include"Engine/Utils/Freelist.h"
#include"Engine/Utils/TypeAlias.h"

using namespace ENGINE::UTILITY;

namespace ENGINE {
	namespace ECS {

		struct Entity {
			uint64_t ruid; // Runtime unique id for accessing Components
			EUID uid; // Engine unique id for Serialization and Referencing
			std::string name;

			Entity() = default;
			Entity(uint64_t ruid, EUID uid, std::string name) : ruid(ruid), uid(uid), name(name) {}
		};

		class EntityManager {
		public:
			EntityManager();

			EUID CreateEntity(std::string name);
			void DestroyEntity(EUID entity);
			bool IsEntityAlive(EUID entity);
			Ref<Entity> GetEntity(EUID entity);
			Ref<Entity> GetEntity(uint64_t entity);
			std::vector<EUID> GetEntities();
			size_t AliveCount();

		private:
			void AddRuntimeIDs();
			std::unordered_map<EUID, Entity> m_entities{};
			FreeList<uint64_t> m_RuntimeIDs;
			size_t entitiesAlive;
		};

	}
}

#endif