#ifndef REGISTERY_CLASS_H
#define REGISTERY_CLASS_H

#include"EntityManager.h"
#include"ComponentManager.h"
#include"SystemManager.h"
#include"RenderItem.h"
#include"View.h"

class Registry {
public:
	Registry();

	std::vector<Entity> GetAllEntities() {
		std::vector<Entity> alive;

		for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
			if (entityManager.IsEntityAlive(i)) {
				alive.push_back(i);
			}
		}

		return alive;
	}

	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	std::vector<Entity> GetEntitiesWithComponents(ComponentType types[], int length);
	std::vector<Entity> GetEntitiesWithComponents(std::vector<ComponentType> types);

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

	template<typename T>
	bool Has(Entity entity) {
		return componentManager.HasComponent<T>(entity);
	}

	template<typename T>
	ComponentArray<T>* GetComponentArray() {
		return componentManager.GetComponentArray(GetComponentType<T>());
	}

	template<typename... Components>
	View view() {
		return View(&componentManager, &entityManager, GetAllEntities()).Has<Components...>();
	}

	std::vector<RenderItem> CreateRenderList();

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};

#endif