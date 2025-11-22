#ifndef LUA_COMPONENT_MANAGER_H
#define LUA_COMPONENT_MANAGER_H

#include"Core/SparceSet.h"
#include"sol/sol.hpp"
#include"ECSData.h"

#include<unordered_map>
#include<string>

using Entity = uint32_t;

class ComponentManagerLua {
public:
	class ComponentArray {
	public:
		ComponentArray(Entity capacity, uint8_t paginationSize) : set(capacity, paginationSize) {}
		ComponentArray() : set(MAX_ENTITIES, 5) {}

		void Add(Entity entity, sol::table component) {
			set.insert(entity, component);
		}

		void Remove(Entity entity) {
			set.remove(entity);
		}

		sol::table& Get(Entity entity) {
			return set.get(entity);
		}

		bool Has(Entity entity) {
			return set.get(entity) != nullptr;
		}

		std::vector<Entity> GetAssignedEntities() {
			std::vector<Entity> entities = {};

			for (sol::table& c : set.getAll()) {
				entities.push_back(c["Enitiy"].get<int>());
			}

			return entities;
		}

		void Clear() {
			set.clear();
		}

		uint32_t Size() { return set.size; }

	private:
		SparseSet<sol::table> set;
	};

public:
	ComponentManagerLua() = default;

	void RegisterComponent(std::string name, sol::table component) {
		if (ComponentRegistryCheck(name, false)) {
			EXCEPTION(("Component already registered! Component: " + name).c_str());
			return;
		}

		component.set("Entity", -1);

		registeredComponents[name] = component;
		components[name] = ComponentArray();
	}

	sol::table GetCopyOfDefaultComponent(std::string name) {
		sol::table copy = sol::table(registeredComponents[name].lua_state(), sol::create);

		for (auto& [key, value] : registeredComponents[name]) {
			copy[key] = value;
		}

		return copy;
	}

	void AddComponent(Entity entity, std::string componentName, sol::table override) {
		if (!ComponentRegistryCheck(componentName)) return;

		sol::table component = GetCopyOfDefaultComponent(componentName);

		for (auto& [key, value] : override) {
			component[key] = value;
		}

		component["Entity"] = entity;

		components[componentName].Add(entity, component);
	}

	std::vector<Entity> GetEntitiesWithComponent(std::string componentName) {
		if (!ComponentRegistryCheck(componentName)) return {};
		return components[componentName].GetAssignedEntities();
	}

	ComponentArray* GetComponentArray(std::string componentName) {
		if (!ComponentRegistryCheck(componentName)) return nullptr;
		return &components[componentName];
	}

	bool HasComponent(Entity entity, std::string name) {
		if (!ComponentRegistryCheck(name)) return false;

		return components[name].Has(entity);
	}

	sol::table& GetComponent(Entity entity, std::string name) {
		ComponentRegistryCheck(name);
		return components[name].Get(entity);
	}

private:
	std::unordered_map<std::string, sol::table> registeredComponents;
	std::unordered_map<std::string, ComponentArray> components;

	bool ComponentRegistryCheck(std::string component, bool throwException = true) {
		if (!registeredComponents.contains(component)) {
			if (throwException) EXCEPTION(("Component is not registered! Component: " + component).c_str());
			return false;
		}

		return true;
	}
};

#endif