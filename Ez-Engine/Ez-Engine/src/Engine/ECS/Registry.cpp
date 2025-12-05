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
			transformBinding.addFunc = [this](uint32_t id) -> void {
				AddComponent<TransformComponent>(id, {});
			};

			RegisterBinding<TransformComponent>(transformBinding);
		}

		Entity Registry::CreateEntity(const char* name) {
			Entity id = entityManager.CreateEntity(name);
			EventBus::Emit<OnEntityCreated>({ id });
			return id;
		}

		void Registry::DestroyEntity(Entity entity) {
			entityManager.DestroyEntity(entity);
			componentManager.EntityDestroyed(entity);
			EventBus::Emit<OnEntityDestroyed>({ entity });
		}

		std::vector<Entity> Registry::GetEntitiesWithComponents(ComponentType types[], int length) {

			std::vector<Entity> entities = {};

			for (size_t i = 0; i < MAX_ENTITIES; i++)
			{
				if (!entityManager.IsEntityAlive(i)) continue;

				bool hasAllComponents = true;

				for (unsigned int i = 0; i < length; i++) {
					ComponentType type = types[i];

					if (!componentManager.HasComponent(i, type)) {
						hasAllComponents = false;
						break;
					}
				}

				if (hasAllComponents) entities.push_back(i);
			}

			return entities;
		}

		std::vector<Entity> Registry::GetEntitiesWithComponents(std::vector<ComponentType> types) {

			std::vector<Entity> entities = {};

			for (size_t i = 0; i < MAX_ENTITIES; i++)
			{
				if (!entityManager.IsEntityAlive(i)) continue;

				bool hasAllComponents = true;

				for (ComponentType type : types) {
					if (!componentManager.HasComponent(i, type)) {
						hasAllComponents = false;
						break;
					}
				}

				if (hasAllComponents) entities.push_back(i);
			}

			return entities;
		}

		std::vector<IComponent*> Registry::GetComponentsOfEntity(Entity entity) {
			std::vector<IComponentArray*> componentArrays = componentManager.GetAllComponentArrays();

			std::vector<IComponent*> entityComponents{};

			for (IComponentArray* arr : componentArrays) {
				if (arr->Has(entity)) {
					entityComponents.push_back(arr->GetBase(entity));
				}
			}

			return entityComponents;
		}

		std::vector<RENDERER::RenderItem> Registry::CreateRenderList() {
			std::vector<RENDERER::RenderItem> renderList{};

			for (Entity e : view().Has<TransformComponent, MeshComponent, MaterialComponent>().List()) {
				MeshComponent& m = GetComponent<MeshComponent>(e);
				TransformComponent& t = GetComponent<TransformComponent>(e);
				MaterialComponent& mt = GetComponent<MaterialComponent>(e);

				renderList.push_back(RENDERER::RenderItem(t, mt, m));
			}

			return renderList;
		}
	}
}