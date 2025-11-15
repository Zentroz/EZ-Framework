#ifndef REGISTERY_CLASS_H
#define REGISTERY_CLASS_H

#include"EntityManager.h"
#include"ComponentManager.h"
#include"SystemManager.h"

class Registry {
public:
	Registry();

	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	Entity* GetEntitiesWithComponent(ComponentType types[]);

	template<typename T>
	void AddComponent(T* component) {
		componentManager.AddComponent(0, component);
	}

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};

#endif