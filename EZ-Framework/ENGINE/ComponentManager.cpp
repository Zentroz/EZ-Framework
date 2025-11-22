#include"Engine/ComponentManager.h"

bool ComponentManager::HasComponent(Entity entity, ComponentType type) {
	ComponentArray<IComponent>* componentArray = GetComponentArray<IComponent>();

	return componentArray->Has(entity);
}
