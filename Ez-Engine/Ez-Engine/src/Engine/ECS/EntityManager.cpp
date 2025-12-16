#include"EntityManager.h"

namespace ENGINE {
	namespace ECS {

		EntityManager::EntityManager() : entitiesAlive(0) {
			for (uint64_t i = 0; i < 5000; i++) m_RuntimeIDs.push(i);
		}

		EUID EntityManager::CreateEntity(std::string name) {
			EUID uid;

			if (IsEntityAlive(uid)) {
				LOG_ERROR("Generated identical uid.");
				return EUID(true);
			}

			if (name.empty()) name = "New Entity";

			m_entities[uid] = Entity(m_RuntimeIDs.pop(), uid, name);

			entitiesAlive++;

			if (m_RuntimeIDs.Size() == entitiesAlive) AddRuntimeIDs();

			return uid;
		}

		void EntityManager::DestroyEntity(EUID uid) {
			if (!IsEntityAlive(uid)) return;
			m_RuntimeIDs.push(m_entities[uid].ruid);
			m_entities.erase(uid);
		}

		bool EntityManager::IsEntityAlive(EUID uid) {
			return m_entities.contains(uid);
		}

		Ref<Entity> EntityManager::GetEntity(EUID uid) {
			if (uid.isNull()) return std::nullopt;
			if (!IsEntityAlive(uid)) EXCEPTION("Can't return an entity that doesn't exist.");
			return m_entities[uid];
		}

		Ref<Entity> EntityManager::GetEntity(uint64_t ruid) {
			for (auto& [id, entity] : m_entities) {
				return entity;
			}

			return std::nullopt;
		}

		std::vector<EUID> EntityManager::GetEntities() {
			std::vector<EUID> entities;
			entities.reserve(m_entities.size());

			for (auto [id, entity] : m_entities) {
				entities.push_back(id);
			}

			return entities;
		}

		size_t EntityManager::AliveCount() {
			return entitiesAlive;
		}

		void EntityManager::AddRuntimeIDs() {
			uint64_t start = m_RuntimeIDs.Size();
			uint64_t end = m_RuntimeIDs.Size() + 5000;
			for (uint64_t i = start; i < end; i++) m_RuntimeIDs.push(i);
		}

	}
}