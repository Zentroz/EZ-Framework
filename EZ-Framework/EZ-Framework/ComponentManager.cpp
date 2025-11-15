#include"ComponentManager.h"

ComponentArray<IComponent>* ComponentManager::GetComponentArray(ComponentType type) {
	if (type >= m_componentCount) return nullptr;

	ComponentArray<IComponent>* componentArray = &m_componentArrays[type];

	return componentArray;
}

bool ComponentManager::HasComponent(unsigned int entity, ComponentType type) {
	ComponentArray<IComponent>* componentArray = GetComponentArray(type);

	return componentArray->GetData(entity) != nullptr;
}
