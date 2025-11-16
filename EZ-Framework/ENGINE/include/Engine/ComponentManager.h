#ifndef COMPONENT_MANAGER_CLASS_H
#define COMPONENT_MANAGER_CLASS_H

#include"Core/SparceSet.h"

#include<iostream>
#include<unordered_map>
#include<string>
#include<bitset>
#include"ECSData.h"
#include"ECSComponents.h"


template<typename T>
class ComponentArray {
public:
	virtual ~ComponentArray() = default;

	ComponentArray() : set(MAX_ENTITIES) {}

	void InsertData(unsigned int entity, T* component) {
		set.insert(entity, component);
	}

	void RemoveData(unsigned int entity) {
		set.remove(entity);
	}

	T* GetData(unsigned int entity) {
		return set.get(entity);
	}

	std::vector<T*> GetAllData() {
		return set.getAll();
	}

	void Clear() {
		set.clear();
	}

	int Size() {
		return set.size;
	}

private:
	SparseSet<T> set;
};

class ComponentManager {
public:
	ComponentManager() = default;

	inline int GetComponentCount() const { return m_componentCount; }

	bool HasComponent(unsigned int entity, ComponentType type);
	ComponentArray<IComponent>* GetComponentArray(ComponentType type);

	template<typename T>
	void RegisterComponent();

	template<typename T>
	ComponentType GetComponentType();

	template<typename T>
	std::vector<T*> GetAllComponentsOfType() {
		ComponentType type = GetComponentType<T>();
		ComponentArray<IComponent>* componentArray = GetComponentArray(type);
		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
			return {};
		}
		std::vector<T*> components;
		for (IComponent* comp : componentArray->GetAllData()) {
			components.push_back(static_cast<T*>(comp));
		}

		return components;
	}

	template<typename T>
	void AddComponent(unsigned int entity, T* component) {
		ComponentType type = GetComponentType<T>();
		ComponentArray<IComponent>* componentArray = GetComponentArray(type);
		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
			return;
		}

		component->entity = entity;

		componentArray->InsertData(entity, static_cast<IComponent*>(component));
	}

	template<typename T>
	void RemoveComponent(unsigned int entity) {
		ComponentType type = GetComponentType<T>();
		ComponentArray<IComponent>* componentArray = GetComponentArray(type);
		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
			return;
		}

		componentArray->RemoveData(entity);
	}

	template<typename T>
	T* GetComponent(Entity entity) {
		ComponentType type = GetComponentType<T>();
		ComponentArray<IComponent>* componentArray = GetComponentArray(type);

		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
			
			return {};
		}

		T* component = static_cast<T*>(componentArray->GetData(entity));

		return component;
	}

	template<typename T>
	bool HasComponent(unsigned int entity) {
		ComponentType type = GetComponentType<T>();

		ComponentArray<IComponent>* componentArray = GetComponentArray(type);

		return componentArray->GetData(entity) != nullptr;
	}


private:
	std::unordered_map<std::string, ComponentType> m_componentTypes{};
	std::vector<ComponentArray<IComponent>> m_componentArrays{};
	ComponentType m_componentCount = 0;
};

template<typename T>
void ComponentManager::RegisterComponent() {
	std::string typeName = typeid(T).name();

	if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
		return;
	}

	ComponentType newType = static_cast<ComponentType>(m_componentCount);

	m_componentTypes.insert({ typeName, newType });
	m_componentArrays.push_back({});
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

#endif