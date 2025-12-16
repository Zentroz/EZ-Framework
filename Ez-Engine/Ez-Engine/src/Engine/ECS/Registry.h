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
			using RUID = uint64_t;
			using ComponentAddFn = void(*)(uint64_t, ComponentManager&);

			struct ComponentBinding {
			public:
				ComponentType type;
				std::function<void(EUID)> addFunc;
				std::function<void(EUID)> hasFunc;
			};

		public:
			Registry();

			void Reset() {

			}

			void RegisterComponent();

			template <typename Func>
			void Each(Func func) {
				for (EUID id : entityManager.GetEntities()) func(id);
			}

			std::vector<uint64_t> GetAllEntityRUIDs();
			std::vector<Entity> GetAllEntities();

			View view() {
				return View(&componentManager, &entityManager, GetAllEntityRUIDs());
			}

			EUID CreateEntity(std::string name);
			void DestroyEntity(EUID id);
			Ref<Entity> GetEntity(EUID id);

			// For View
			std::vector<EUID> GetEntitiesWithComponents(ComponentType types[], int length);
			std::vector<EUID> GetEntitiesWithComponents(std::vector<ComponentType> types);

			// For Rendering Inspector UI
			std::vector<IComponent*> GetComponentsOfEntity(EUID id);
			std::vector<ComponentType> GetAllComponentTypesOfEntity(EUID id);

			template<typename T>
			ComponentType GetComponentType(T) {
				return componentManager.GetComponentType<T>();
			}

			template<typename T>
			ComponentType GetComponentType() {
				return componentManager.GetComponentType<T>();
			}

			// Component Manager wrappers
			template<typename T>
			void AddComponent(EUID id) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				if (!entity) return;
				RUID ruid = entity->get().ruid;
				componentManager.AddComponent(ruid);
				EventBus::Emit<ComponentAddedEvent<T>>(ComponentAddedEvent<T>(ruid, GetComponent<T>(ruid), this));
			}

			template<typename T>
			T& GetComponent(EUID id) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				return componentManager.GetComponent<T>(entity->get().ruid);
			}

			template<typename T>
			bool Has(EUID id) {
				Ref<Entity> entity = entityManager.GetEntity(id);
				if (!entity) return false;
				return componentManager.HasComponent<T>(entity->get().ruid);
			}

			template<typename T>
			void AddComponent(RUID id, T component) {
				componentManager.AddComponent(id, component);
				EventBus::Emit<ComponentAddedEvent<T>>(ComponentAddedEvent<T>(id, GetComponent<T>(id), this));
			}

			template<typename T>
			T& GetComponent(RUID id) {
				return componentManager.GetComponent<T>(id);
			}

			template<typename T>
			bool Has(RUID id) {
				return componentManager.HasComponent<T>(id);
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