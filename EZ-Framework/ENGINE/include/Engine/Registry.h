#ifndef REGISTERY_CLASS_H
#define REGISTERY_CLASS_H

#include"EntityManager.h"
#include"ComponentManager.h"
#include"LuaComponentManager.h"
#include"SystemManager.h"
#include"RenderItem.h"
#include"View.h"
#include"ViewLua.h"
#include"EventBus.h"

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

	View view() {
		return View(&componentManager, GetAllEntities());
	}

	ViewLua viewLua() {
		return ViewLua(&componentManagerLua, GetAllEntities());
	}

	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	std::vector<Entity> GetEntitiesWithComponents(ComponentType types[], int length);
	std::vector<Entity> GetEntitiesWithComponents(std::vector<ComponentType> types);

	void RegisterLuaComponent(std::string name, sol::table component) {
		componentManagerLua.RegisterComponent(name, component);
	}
	void AddLuaComponent(Entity entity, std::string name, sol::table override) {
		componentManagerLua.AddComponent(entity, name, override);
	}
	sol::table& GetLuaComponent(Entity entity, std::string name) {
		return componentManagerLua.GetComponent(entity, name);
	}

	template<typename T>
	ComponentType GetComponentType() {
		return componentManager.GetComponentType<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		componentManager.AddComponent(entity, component);
		EventBus::Emit<ComponentAddedEvent<T>>(ComponentAddedEvent<T>(entity, GetComponent<T>(entity), this));
	}

	template<typename T>
	T& GetComponent(Entity entity) {
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

	std::vector<RenderItem> CreateRenderList();

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	ComponentManagerLua componentManagerLua;
	SystemManager systemManager;
};

#endif