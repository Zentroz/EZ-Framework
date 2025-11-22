#ifndef COMPONENT_MANAGER_CLASS_H
#define COMPONENT_MANAGER_CLASS_H

#include"Core/SparceSet.h"

#include<iostream>
#include<unordered_map>
#include<string>
#include"ECSData.h"
#include"ECSComponents.h"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void RemoveData(Entity entity) = 0;
	virtual bool Has(Entity entity) = 0;
	virtual std::vector<Entity> GetAssignedEntities() = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray() : set(MAX_ENTITIES, 10) {}

	void InsertData(uint32_t entity, T component) {
		set.insert(entity, component);
	}

	void RemoveData(uint32_t entity) override {
		set.remove(entity);
	}

	T& GetData(uint32_t entity) {
		return set.get(entity);
	}

	std::vector<T> GetAllData() const {
		return set.getAll();
	}

	void Clear() {
		set.clear();
	}

	bool Has(Entity entity) override { return set.has(entity); }

	std::vector<Entity> GetAssignedEntities() override {
		std::vector<Entity> entities;

		for (T component : set.getAll()) {
			entities.push_back(component.entity);
		}

		return entities;
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

	bool HasComponent(Entity entity, ComponentType type);

	template<typename T>
	void RegisterComponent();

	template<typename T>
	ComponentType GetComponentType();

	template<typename T>
	void AddComponent(Entity entity, T component) {
		ComponentType type = GetComponentType<T>();
		ComponentArray<T>* componentArray = GetComponentArray<T>();
		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
			return;
		}

		component.entity = entity;

		componentArray->InsertData(entity, component);
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
	T& GetComponent(Entity entity) {
		ComponentArray<T>* componentArray = GetComponentArray<T>();

		if (componentArray == nullptr) {
			EXCEPTION("Component Array not found.");
		}

		T& data = componentArray->GetData(entity);

		return static_cast<T&>(data);
	}

	template<typename T>
	bool HasComponent(uint32_t entity) {
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
	std::unordered_map<std::string, ComponentType> m_componentTypes{};
	std::vector<IComponentArray*> m_componentArrays{};
	ComponentType m_componentCount = 0;
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

#endif