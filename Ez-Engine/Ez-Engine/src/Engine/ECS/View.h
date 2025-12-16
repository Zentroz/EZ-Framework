#ifndef REGISTRY_VIEW_H
#define REGISTRY_VIEW_H

#include<tuple>

#include"Engine/ECS/EntityManager.h"
#include"Engine/ECS/ComponentManager.h"


namespace ENGINE {
	namespace ECS {

		struct View {
		public:
			View(ComponentManager* componentManager, EntityManager* entityManager, std::vector<uint64_t> result) :
				componentManager(componentManager), entityManager(entityManager), result(result) { };

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


				std::vector<uint64_t> result;
				for (uint64_t e : smallest->GetAssignedEntities()) {
					if (((c->HasComponent<Components>(e)) && ...)) {
						result.push_back(e);
					}
				}

				this->result.clear();

				return View(componentManager, entityManager, result);
			}

			std::vector<uint64_t> List() const { return result; }

		private:
			ComponentManager* componentManager;
			EntityManager* entityManager;
			std::vector<uint64_t> result;
		};

	}
}

#endif