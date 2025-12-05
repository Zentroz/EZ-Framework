#include"Editor/EditorLayer.h"

void EditorLayer::OnAttach() {
	// Component Items
	componentItems[ENGINE::Engine::Get().Registry()->GetComponentType<ENGINE::ECS::TransformComponent>()] = ComponentListItem("Transform", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::TransformComponent>(entity, {}); }, EDITOR::DrawTransform);
	componentItems[ENGINE::Engine::Get().Registry()->GetComponentType<ENGINE::ECS::MeshComponent>()] = ComponentListItem("Mesh", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::MeshComponent>(entity, ENGINE::ECS::MeshComponent()); }, EDITOR::DrawMeshComponent);
	componentItems[ENGINE::Engine::Get().Registry()->GetComponentType<ENGINE::ECS::MaterialComponent>()] = ComponentListItem("Material", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::MaterialComponent>(entity, ENGINE::ECS::MaterialComponent()); }, EDITOR::DrawMaterialComponent);
}
void EditorLayer::OnDetach() {
}
void EditorLayer::OnUpdate() {
	ENGINE::Engine::Get().sceneViewportSize = float2(sceneScreenSize.x, sceneScreenSize.y);
}
void EditorLayer::OnRenderImGui() {
	RenderUI();
}

void EditorLayer::RenderUI() {

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Editor", nullptr, windowFlags)) {

		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Asset")) {

				if (ImGui::MenuItem("Load Asset")) {
					//OpenFileDialog("", engine.GetBackend()->GetHwnd());
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows")) {
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::Begin("Scene")) {
			sceneScreenSize = ImGui::GetContentRegionAvail();
			ID3D11ShaderResourceView* srv = ENGINE::Engine::Get().Backend()->GetCustomRenderTarget()->GetSRV();
			ImGui::Image((ImTextureID)srv, ImVec2(sceneScreenSize.x, sceneScreenSize.y));
			//ImGui::Text("No Camera To Render!");
			ImGui::End();
		}

		if (ImGui::Begin("Hierarchy")) {
			if (ImGui::BeginPopupContextWindow("HierarchyContextMenu"))
			{
				if (ImGui::MenuItem("Create Entity"))
				{
					ENGINE::Engine::Get().Registry()->CreateEntity();
				}
				if (ImGui::MenuItem("Create Mesh"))
				{
					Entity mesh = ENGINE::Engine::Get().Registry()->CreateEntity();
					ENGINE::Engine::Get().Registry()->AddComponent(mesh, ENGINE::ECS::TransformComponent());
					ENGINE::Engine::Get().Registry()->AddComponent(mesh, ENGINE::ECS::MeshComponent(ENGINE::Engine::Get().AssetManager()->GetIdFromName("cube.obj")));
					ENGINE::Engine::Get().Registry()->AddComponent(mesh, ENGINE::ECS::MaterialComponent(ENGINE::Engine::Get().AssetManager()->GetIdFromName("default.hlsl"), ENGINE::Engine::Get().AssetManager()->GetIdFromName("White.png")));
				}

				ImGui::EndPopup();
			}

			for (const auto& item : ENGINE::Engine::Get().Registry()->GetAllEntitiesData())
			{
				ImGui::PushID(item->id);

				if (ImGui::MenuItem(item->name.c_str())) {
					selectedEntity = item->id;
				}

				if (ImGui::BeginPopupContextItem("EntityContextMenu"))
				{
					selectedEntity = item->id;
					if (ImGui::MenuItem("Delete Entity"))
					{
						ENGINE::Engine::Get().Registry()->DestroyEntity(selectedEntity);
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
			ImGui::End();
		}


		if (ImGui::Begin("Inspector")) {
			// Rendering Components
			EDITOR::ComponentUIDrawInput componentDrawInput = { selectedEntity, ENGINE::Engine::Get().Registry(), ENGINE::Engine::Get().AssetManager(), textInput, &floatInput, float2Input, float3Input, float4Input };
			std::vector<ComponentType> components = ENGINE::Engine::Get().Registry()->GetAllComponentTypesOfEntity(selectedEntity);

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
						component.addFunc(ENGINE::Engine::Get().Registry(), selectedEntity);
					}
				}
				ImGui::EndPopup();
			}

			ImGui::End();
		}

		if (ImGui::Begin("Assets")) {

			if (ImGui::Button("Load Asset")) {
				ImGui::OpenPopup("LoadAssetContext");
			}

			if (ImGui::BeginPopup("LoadAssetContext"))
			{
				ImGui::Text("Path: ");
				ImGui::SameLine();
				ImGui::InputText("##LoadAssetInput", textInput, IM_ARRAYSIZE(textInput));

				if (ImGui::Button("Load")) {
					// Load Asset
					ENGINE::Engine::Get().AssetManager()->LoadAsset(textInput);
					ZeroMemory(textInput, ARRAYSIZE(textInput));
				}

				ImGui::EndPopup();
			}

			for (auto& asset : ENGINE::Engine::Get().AssetManager()->GetLoadedAssets()) {
				if (ImGui::Selectable(asset->name.c_str())) {

				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set the payload data
					uint16_t itemID = asset->id;
					ImGui::SetDragDropPayload("ASSET_ID_PAYLOAD", &itemID, sizeof(uint16_t));

					// Render a preview of the dragged item
					ImGui::Text("Selected Asset: %s", asset->name.c_str());

					ImGui::EndDragDropSource();
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Console")) {
			if (ImGui::BeginPopupContextWindow("ConsoleContextMenu"))
			{
				if (ImGui::MenuItem("Clear"))
				{
					ENGINE::Logger::Clear();
				}

				ImGui::EndPopup();
			}

			for (const auto& msg : ENGINE::Logger::GetLogs())
			{
				ImVec4 color;
				switch (msg.type) {
				case ENGINE::LogType::Info:    color = ImVec4(1, 1, 1, 1); break;
				case ENGINE::LogType::Warning: color = ImVec4(1, 1, 0, 1); break;
				case ENGINE::LogType::Error:   color = ImVec4(1, 0.4f, 0.4f, 1); break;
				}

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextWrapped("%s", msg.msg.c_str());
				ImGui::PopStyleColor();
			}
			ImGui::End();
		}

		ImGui::End();
	}
}