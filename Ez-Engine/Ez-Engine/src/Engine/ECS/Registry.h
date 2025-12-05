#ifndef REGISTERY_CLASS_H
#define REGISTERY_CLASS_H

#include"Engine/Core/EventBus.h"
#include"Engine/Render/Frontend/RenderItem.h"
#include"Engine/ECS/EntityManager.h"
#include"Engine/ECS/ComponentManager.h"
#include"Engine/ECS/SystemManager.h"
#include"Engine/ECS/View.h"

namespace ENGINE {
	namespace ECS {

		class Registry {
		private:
			struct ComponentBinding {
			public:
				ComponentType type;
				std::function<void(uint32_t)> addFunc;
				std::function<void(uint32_t)> hasFunc;
			};

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

			std::vector<GameEntity*> GetAllEntitiesData() {
				std::vector<GameEntity*> alive;

				for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
					if (entityManager.IsEntityAlive(i)) {
						alive.push_back(GetGameEntity(i));
					}
				}

				return alive;
			}

			View view() {
				return View(&componentManager, GetAllEntities());
			}

			Entity CreateEntity(const char* name = nullptr);
			void DestroyEntity(Entity entity);
			GameEntity* GetGameEntity(Entity entity) { return entityManager.GetGameEntity(entity); }
			std::vector<Entity> GetEntitiesWithComponents(ComponentType types[], int length);
			std::vector<Entity> GetEntitiesWithComponents(std::vector<ComponentType> types);

			std::vector<IComponent*> GetComponentsOfEntity(Entity entity);

			std::vector<ComponentType> GetAllComponentTypesOfEntity(Entity entity) {
				std::vector<ComponentType> types = componentManager.GetAllComponentTypes();
				std::vector<ComponentType> hasTypes;
				IComponentArray* arr = nullptr;

				for (auto type : types) {
					arr = componentManager.GetComponentArray(type);
					if (arr->Has(entity)) hasTypes.push_back(type);
				}

				return hasTypes;
			}

			template<typename T>
			ComponentType GetComponentType(T) {
				return componentManager.GetComponentType<T>();
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

			std::vector<RENDERER::RenderItem> CreateRenderList();

			template<typename T>
			void RegisterBinding(ComponentBinding binding) {
				ComponentType cType = GetComponentType<T>();
				binding.type = cType;
			}

		private:
			EntityManager entityManager;
			ComponentManager componentManager;

			std::unordered_map<std::string, ComponentBinding> componentBindings;
		};

	}
}

#endif