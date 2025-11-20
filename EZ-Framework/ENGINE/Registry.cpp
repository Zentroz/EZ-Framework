#include"Engine/Registry.h"

Registry::Registry() {
	componentManager.RegisterComponent<TransformComponent>();
	componentManager.RegisterComponent<MeshComponent>();
	componentManager.RegisterComponent<MaterialComponent>();
}

Entity Registry::CreateEntity() {
	return entityManager.CreateEntity();
}

void Registry::DestroyEntity(Entity entity) {
	entityManager.DestroyEntity(entity);
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

std::vector<RenderItem> Registry::CreateRenderList() {
	std::vector<RenderItem> renderList{};

	for (Entity e : view<TransformComponent, MeshComponent, MaterialComponent>().List()) {
		MeshComponent* m = GetComponent<MeshComponent>(e);
		TransformComponent* t = GetComponent<TransformComponent>(e);
		MaterialComponent* mt = GetComponent<MaterialComponent>(e);

		renderList.push_back(RenderItem(m->path, mt->shaderPath, t->position, t->scale, t->rotation));
	}

	return renderList;
}