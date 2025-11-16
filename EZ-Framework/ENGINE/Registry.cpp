#include"Engine/Registry.h"

Registry::Registry() {
	componentManager.RegisterComponent<TransformComponent>();
	componentManager.RegisterComponent<ScriptComponent>();
}

Entity Registry::CreateEntity() {
	return entityManager.CreateEntity();
}

void Registry::DestroyEntity(Entity entity) {
	entityManager.DestroyEntity(entity);
}

std::vector<Entity> Registry::GetEntitiesWithComponent(ComponentType types[], int length) {

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