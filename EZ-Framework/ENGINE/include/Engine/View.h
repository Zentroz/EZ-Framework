#ifndef REGISTRY_VIEW_H
#define REGISTRY_VIEW_H

#include<tuple>
#include"ComponentManager.h"

struct View {
public:
	View(ComponentManager* componentManager, std::vector<Entity> result) : componentManager(componentManager), result(result) {};

	template<typename... Components>
	View Has() {
		ComponentManager* c = this->componentManager;

		auto pools = std::tuple{ c->GetComponentArray<Components>()... };

		IComponentArray* smallest = nullptr;
		size_t smallestSize = SIZE_MAX;

		std::apply([&](auto... p) {
			(([&]() {
				if (p->Size() < smallestSize) {
					smallest = p;   // <-- no cast needed
					smallestSize = p->Size();
				}
				}()), ...);
			}, pools);


		std::vector<Entity> result;
		for (Entity e : smallest->GetAssignedEntities()) {
			if (((c->HasComponent<Components>(e)) && ...)) {
				result.push_back(e);
			}
		}

		this->result.clear();

		return View(componentManager, result);
	}

	std::vector<Entity> List() const { return result; }

private:
	ComponentManager* componentManager;
	std::vector<Entity> result;
};

#endif