#include"Registry.h"

namespace ENGINE {
	namespace ECS {

		Registry::Registry() {
			componentManager.RegisterComponent<TransformComponent>();
			componentManager.RegisterComponent<MeshComponent>();
			componentManager.RegisterComponent<MaterialComponent>();
			componentManager.RegisterComponent<ColliderComponent>();
			componentManager.RegisterComponent<RigidBodyComponent>();
			componentManager.RegisterComponent<KinematicBody>();
			componentManager.RegisterComponent<ParticleComponent>();
			componentManager.RegisterComponent<ScriptComponent>();

			ComponentBinding transformBinding;
			transformBinding.addFunc = [this](EUID id) -> void {
				AddComponent<TransformComponent>(id, {});
			};

			RegisterBinding<TransformComponent>(transformBinding);
		}

		EUID Registry::CreateEntity(std::string name) {
			EUID id = entityManager.CreateEntity(name);
			EventBus::Emit<OnEntityCreated>({ id });
			return id;
		}

		void Registry::DestroyEntity(EUID id) {
			if (!entityManager.IsEntityAlive(id) || id.isNull()) return;
			componentManager.EntityDestroyed(entityManager.GetEntity(id)->get().ruid);
			entityManager.DestroyEntity(id);
			EventBus::Emit<OnEntityDestroyed>({ id });
		}

		Ref<Entity> Registry::GetEntity(EUID id) {
			return entityManager.GetEntity(id);
		}

		std::vector<Entity> Registry::GetAllEntities() {
			std::vector<Entity> alive;

			Each([&](EUID id) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				if (entity) alive.push_back(entity->get());
			});

			return alive;
		}

		std::vector<uint64_t> Registry::GetAllEntityRUIDs() {
			std::vector<uint64_t> alive;

			Each([&](EUID id) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				if (entity) alive.push_back(entity->get().ruid);
			});

			return alive;
		}

		// For rendering Inspector UI
		std::vector<IComponent*> Registry::GetComponentsOfEntity(EUID id) {
			std::vector<IComponentArray*> componentArrays = componentManager.GetAllComponentArrays();

			std::vector<IComponent*> entityComponents{};

			for (IComponentArray* arr : componentArrays) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				if (!entity) continue;
				RUID ruid = entity->get().ruid;
				if (arr->Has(ruid)) {
					entityComponents.push_back(arr->GetBase(ruid));
				}
			}

			return entityComponents;
		}

		std::vector<ComponentType> Registry::GetAllComponentTypesOfEntity(EUID id) {
			std::vector<ComponentType> types = componentManager.GetAllComponentTypes();
			std::vector<ComponentType> hasTypes;
			IComponentArray* arr = nullptr;

			Ref<Entity> entity = entityManager.GetEntity(id);
			if (!entity) return {};

			for (auto type : types) {
				arr = componentManager.GetComponentArray(type);
				if (arr->Has(entity->get().ruid)) hasTypes.push_back(type);
			}

			return hasTypes;
		}

		// For View
		std::vector<EUID> Registry::GetEntitiesWithComponents(ComponentType types[], int length) {

			std::vector<EUID> entities = {};

			Each([&](EUID id)
				{
					bool hasAllComponents = true;

					for (uint16_t i = 0; i < length; i++) {
						Ref<Entity> entity = entityManager.GetEntity(id);
						if (!entity) continue;

						ComponentType type = types[i];

						if (!componentManager.HasComponent(entity->get().ruid, type)) {
							hasAllComponents = false;
							break;
						}
					}

					if (hasAllComponents) entities.push_back(id);
				}
			);

			return entities;
		}

		std::vector<EUID> Registry::GetEntitiesWithComponents(std::vector<ComponentType> types) {

			std::vector<EUID> entities = {};

			Each([&](EUID id)
				{
					bool hasAllComponents = true;

					for (uint16_t i = 0; i < types.size(); i++) {
						Ref<Entity> entity = entityManager.GetEntity(id);
						if (!entity) continue;

						ComponentType type = types[i];

						if (!componentManager.HasComponent(entity->get().ruid, type)) {
							hasAllComponents = false;
							break;
						}
					}

					if (hasAllComponents) entities.push_back(id);
				}
			);

			return entities;
		}


		std::vector<RENDERER::RenderItem> Registry::CreateRenderList() {
			std::vector<RENDERER::RenderItem> renderList{};

			for (RUID e : view().Has<TransformComponent, MeshComponent, MaterialComponent>().List()) {
				MeshComponent& m = GetComponent<MeshComponent>(e);
				TransformComponent& t = GetComponent<TransformComponent>(e);
				MaterialComponent& mt = GetComponent<MaterialComponent>(e);

				renderList.push_back(RENDERER::RenderItem(t, mt, m));
			}

			return renderList;
		}
	}
}