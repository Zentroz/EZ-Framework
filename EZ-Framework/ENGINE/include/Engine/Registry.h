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
	std::vector<Entity> GetEntitiesWithComponent(ComponentType types[], int length);

	template<typename T>
	ComponentType GetComponentType() {
		return componentManager.GetComponentType<T>();
	}

	template<typename T>
	std::vector<T*> GetAllComponentsOfType() {
		return componentManager.GetAllComponentsOfType<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T* component) {
		componentManager.AddComponent(entity, component);
	}

	template<typename T>
	T* GetComponent(Entity entity) {
		return componentManager.GetComponent<T>(entity);
	}

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};

#endif