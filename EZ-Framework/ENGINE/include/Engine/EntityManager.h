#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include"Core/ErrorHandler.h"

#include<bitset>
#include"ECSData.h"

class EntityManager {
public:
	EntityManager() = default;

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

private:
	std::bitset<MAX_ENTITIES> m_entities{};
	unsigned int entitiesAlive;
};

#endif