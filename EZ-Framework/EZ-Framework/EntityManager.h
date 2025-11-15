#ifndef ENTITY_MANAGER_CLASS_H
#define ENTITY_MANAGER_CLASS_H

#include<bitset>
#include"ECSData.h"

#include"ErrorHandler.h"

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