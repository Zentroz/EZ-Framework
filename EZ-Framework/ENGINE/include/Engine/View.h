#ifndef REGISTRY_VIEW_H
#define REGISTRY_VIEW_H

#include<tuple>
#include"ComponentManager.h"
#include"EntityManager.h"

struct View {
public:
	View(ComponentManager* componentManager, EntityManager* entityManager, std::vector<Entity> result) : componentManager(componentManager), entityManager(entityManager), result(result) {};

	template<typename... Components>
	View Has() {
		ComponentManager* c = this->componentManager;

		auto pools = std::tuple{ c->GetComponentArray<Components>()... };

		ComponentArray<IComponent>* smallest = (std::tuple_element_t<0, decltype(pools)>)nullptr;
		size_t smallestSize = SIZE_MAX;

		std::apply([&](auto... p) {
			(([&]() {
				if (p->Size() < smallestSize) {
					smallest = p;
					smallestSize = p->Size();
				}
				}()), ...);
			}, pools
		);

		std::vector<Entity> result;
		for (Entity e : smallest->GetAssignedEntities()) {
			if (((c->HasComponent<Components>(e)) && ...)) {
				result.push_back(e);
			}
		}

		this->result.clear();

		return View(componentManager, entityManager, result);
	}

	std::vector<Entity> List() const { return result; }

private:
	EntityManager* entityManager;
	ComponentManager* componentManager;
	std::vector<Entity> result;
};

#endif