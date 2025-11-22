#ifndef REGISTRY_VIEW_LUA_H
#define REGISTRY_VIEW_LUA_H

#include"LuaComponentManager.h"

using Entity = uint32_t;

struct ViewLua {
public:
	ViewLua(ComponentManagerLua* componentManager, std::vector<Entity> result) : componentManager(componentManager), result(result) {};

	ViewLua Has(std::vector<sol::object> components) {
		ComponentManagerLua::ComponentArray* smallest = nullptr;
		uint32_t size = -1;

		for (sol::object c : components) {
			ComponentManagerLua::ComponentArray* arr = componentManager->GetComponentArray(c.as<std::string>());

			if (arr->Size() < size) {
				size = arr->Size();
				smallest = arr;
			}
		}

		std::vector<Entity> result;

		for (Entity e : smallest->GetAssignedEntities()) {
			bool hasAll = true;

			for (sol::object c : components) {
				if (!componentManager->HasComponent(e, c.as<std::string>())) {
					hasAll = false;
					break;
				}
			}

			if (hasAll) result.push_back(e);
		}

		this->result.clear();

		return ViewLua(componentManager, result);
	}

	std::vector<Entity> List() const { return result; }

private:
	ComponentManagerLua* componentManager;
	std::vector<Entity> result;
};

#endif