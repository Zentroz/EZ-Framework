#ifndef COMPONENT_MANAGER_CLASS_H
#define COMPONENT_MANAGER_CLASS_H

#include<string>
#include<iostream>
#include<unordered_map>

#include"Engine/Core/SparceSet.h"
#include"Engine/ECS/ECSData.h"
#include"Engine/ECS/ECSComponents.h"
#include"Engine/Utils/EUID.h"

using namespace ENGINE::UTILITY;

namespace ENGINE {
	namespace ECS {

		class IComponentArray {
		public:
			virtual ~IComponentArray() = default;
			virtual void RemoveData(uint64_t entity) = 0;
			virtual bool Has(uint64_t entity) = 0;
			virtual std::vector<uint64_t> GetAssignedEntities() = 0;
			virtual uint64_t Size() = 0;
		};

		template<typename T>
		class ComponentArray : public IComponentArray {
		public:
			ComponentArray() : set(100000, 10) {}

			void InsertData(uint64_t entity, T component) {
				set.insert(entity, component);
			}

			void RemoveData(uint64_t entity) override {
				set.remove(entity);
			}

			T& GetData(uint64_t entity) {
				return set.get(entity);
			}

			std::vector<T> GetAllData() const {
				return set.getAll();
			}

			void Clear() {
				set.clear();
			}

			IComponent* GetBase(uint64_t entity) override {
				return static_cast<IComponent*>(set.get_ptr(entity));
			}

			bool Has(uint64_t entity) override { return set.has(entity); }

			std::vector<uint64_t> GetAssignedEntities() override {
				std::vector<uint64_t> entities;

				for (T& component : set.getAll()) {
					entities.push_back(component.entityID);
				}

				return entities;
			}

			size_t Size() override {
				return set.size;
			}

		private:
			
		};

		class ComponentManager {
		private:
			using EID = uint64_t;
			using ComponentAddFn = void(*)(uint64_t, ComponentManager&);
		public:
			ComponentManager() = default;

			inline int GetComponentCount() const { return m_componentCount; }

			std::vector<ComponentType> GetAllComponentTypes() {
				std::vector<ComponentType> types;

				for (const auto [key, type] : m_componentTypes) {
					types.push_back(type);
				}

				return types;
			}

			bool HasComponent(EID entity, ComponentType type) {
				IComponentArray* componentArray = GetComponentArray(type);

				return componentArray->Has(entity);
			}

			IComponentArray* GetComponentArray(ComponentType type) { return m_componentArrays[type]; }

			void EntityDestroyed(EID e) {
				for (IComponentArray* arr : m_componentArrays) {
					if (arr->Has(e)) {
						arr->RemoveData(e);
					}
				}
			}

			std::vector<IComponentArray*> GetAllComponentArrays() { return m_componentArrays; }

			template<typename T>
			void RegisterComponent();

			template<typename T>
			ComponentType GetComponentType();

			template<typename T>
			void AddComponent(EID entity) {
				ComponentType type = GetComponentType<T>();
				ComponentArray<T>* componentArray = GetComponentArray<T>();
				if (componentArray == nullptr) {
					EXCEPTION("Component Array not found.");
					return;
				}

				T component = T();
				component.entityID = entity;

				componentArray->InsertData(entity, component);
			}

			template<typename T>
			void RemoveComponent(EID entity) {
				ComponentType type = GetComponentType<T>();
				ComponentArray<IComponent>* componentArray = GetComponentArray(type);
				if (componentArray == nullptr) {
					EXCEPTION("Component Array not found.");
					return;
				}

				componentArray->RemoveData(entity);
			}

			template<typename T>
			T& GetComponent(EID entity) {
				ComponentArray<T>* componentArray = GetComponentArray<T>();

				if (componentArray == nullptr) {
					EXCEPTION("Component Array not found.");
				}

				T& data = componentArray->GetData(entity);

				return static_cast<T&>(data);
			}

			template<typename T>
			bool HasComponent(EID entity) {
				ComponentType type = GetComponentType<T>();

				ComponentArray<T>* componentArray = GetComponentArray<T>();

				return componentArray->Has(entity);
			}

			template<typename T>
			ComponentArray<T>* GetComponentArray() {
				ComponentType type = GetComponentType<T>();

				if (type >= m_componentArrays.size()) {
					EXCEPTION("Component is not registered!");
					return nullptr;
				}

				return static_cast<ComponentArray<T>*>(m_componentArrays[type]);
			}


		private:
			std::unordered_map<ComponentType, ComponentAddFn> m_AddFn{};
			std::vector<IComponentArray*> m_componentArrays{};
		};

		template<typename T>
		void ComponentManager::RegisterComponent() {
			static_assert(std::is_base_of<IComponent, T>::value, "Cannot Register Component! It must inherit IComponent");

			std::string typeName = typeid(T).name();

			if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
				return;
			}

			ComponentType newType = static_cast<ComponentType>(m_componentCount);

			m_componentTypes.insert({ typeName, newType });
			m_componentArrays.push_back(new ComponentArray<T>());
			m_componentCount++;
		}

		template<typename T>
		ComponentType ComponentManager::GetComponentType() {
			std::string typeName = typeid(T).name();

			if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
				return m_componentTypes[typeName];
			}

			return -1;
		}

	}
}

#endif