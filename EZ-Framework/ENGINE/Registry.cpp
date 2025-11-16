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

Entity* Registry::GetEntitiesWithComponent(ComponentType types[]) {
	//int length = sizeof(types) / sizeof(types[0]);

	//for (size_t i = 0; i < MAX_ENTITIES; i++)
	//{
	//	for (unsigned int i = 0; i < length; i++) {
	//		ComponentType type = types[i];

	//		entitymana
	//	}
	//}

	return nullptr;
}