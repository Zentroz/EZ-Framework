#include"Editor/Panels/InspectorPanel.h"

InspectorPanel::InspectorPanel() {
	// Component Items
	componentItems[ENGINE::Engine::Get().Registry().GetComponentType<ENGINE::ECS::TransformComponent>()] = ComponentListItem("Transform", [](ENGINE::ECS::Registry& registry, Ref<ENGINE::ECS::Entity> entity) { registry.AddComponent<ENGINE::ECS::TransformComponent>(entity->get().ruid, {}); }, EDITOR::DrawTransform);
	componentItems[ENGINE::Engine::Get().Registry().GetComponentType<ENGINE::ECS::MeshComponent>()] = ComponentListItem("Mesh", [](ENGINE::ECS::Registry& registry, Ref<ENGINE::ECS::Entity> entity) { registry.AddComponent<ENGINE::ECS::MeshComponent>(entity->get().ruid, ENGINE::ECS::MeshComponent()); }, EDITOR::DrawMeshComponent);
	componentItems[ENGINE::Engine::Get().Registry().GetComponentType<ENGINE::ECS::MaterialComponent>()] = ComponentListItem("Material", [](ENGINE::ECS::Registry& registry, Ref<ENGINE::ECS::Entity> entity) { registry.AddComponent<ENGINE::ECS::MaterialComponent>(entity->get().ruid, ENGINE::ECS::MaterialComponent()); }, EDITOR::DrawMaterialComponent);
}

void InspectorPanel::OnRenderImGui(EUID selectedEntity) {
	if (ImGui::Begin("Inspector")) {
		if (!selectedEntity.isNull()) {
			// Rendering Components
			EDITOR::ComponentUIDrawInput componentDrawInput = { selectedEntity, ENGINE::Engine::Get().Registry(), ENGINE::Engine::Get().AssetManager(), textInput, &floatInput, float2Input, float3Input, float4Input };
			std::vector<ComponentType> components = ENGINE::Engine::Get().Registry().GetAllComponentTypesOfEntity(selectedEntity);

			for (auto type : components) {
				if (!componentItems.contains(type)) continue;
				ImGui::PushID(("componentId" + std::to_string(type)).c_str());
				componentItems[type].drawFunc(componentDrawInput);
				ImGui::PopID();
			}

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponentMenu");
			}

			// Define the popup's content
			if (ImGui::BeginPopup("AddComponentMenu"))
			{
				for (const auto [key, component] : this->componentItems) {
					if (ImGui::MenuItem(component.name.c_str()))
					{
						if (!selectedEntity.isNull())
							component.addFunc(ENGINE::Engine::Get().Registry(), ENGINE::Engine::Get().Registry().GetEntity(selectedEntity));
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}