#include"Editor.h"

namespace EDITOR {
	void Editor::Init() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

		engine.Init({ 1920, 1080, "EZ-Editor" });

		// Setup Platform/Renderer backends
		GraphicsDevice* graphics = engine.Get().Backend()->GetGraphicDevice();

		ImGui_ImplWin32_Init(engine.Get().Backend()->GetHwnd());
		ImGui_ImplDX11_Init(graphics->GetDevice(), graphics->GetContext());

		engine.Get().AssetManager()->LoadAsset("Assets/Mesh/cube.obj");
		engine.Get().AssetManager()->LoadAsset("Assets/Shaders/default.hlsl");
		engine.Get().AssetManager()->LoadAsset("Assets/Textures/White.png");

		// Component Items
		componentItems[engine.Get().Registry()->GetComponentType<ENGINE::ECS::TransformComponent>()] = ComponentListItem("Transform", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::TransformComponent>(entity, {}); }, DrawTransform);
		componentItems[engine.Get().Registry()->GetComponentType<ENGINE::ECS::MeshComponent>()] = ComponentListItem("Mesh", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::MeshComponent>(entity, ENGINE::ECS::MeshComponent()); }, DrawMeshComponent);
		componentItems[engine.Get().Registry()->GetComponentType<ENGINE::ECS::MaterialComponent>()] = ComponentListItem("Material", [](ENGINE::ECS::Registry* registry, Entity entity) { registry->AddComponent<ENGINE::ECS::MaterialComponent>(entity, ENGINE::ECS::MaterialComponent()); }, DrawMaterialComponent);
	}
	void Editor::Shutdown() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		engine.Shutdown();
	}
	void Editor::Update() {
		bool run = true;

		while (run) {

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::DockSpaceOverViewport();

			entities = engine.Get().Registry()->GetAllEntitiesData();
			RenderUI();

			//run = engine.Run(float2(sceneScreenSize.x, sceneScreenSize.y));

			engine.SetSwapChainRenderTarget();
			engine.Get().Renderer()->BindLinearSampler();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			//engine.EndFrame();

			engine.Get().Backend()->PresentFrame();

			if (run == false) break;
		}
	}

	void Editor::RenderUI() {

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
				ID3D11ShaderResourceView* srv = engine.Get().Backend()->GetCustomRenderTarget()->GetSRV();
				ImGui::Image((ImTextureID)srv, ImVec2(sceneScreenSize.x, sceneScreenSize.y));
				//ImGui::Text("No Camera To Render!");
				ImGui::End();
			}

			if (ImGui::Begin("Hierarchy")) {
				if (ImGui::BeginPopupContextWindow("HierarchyContextMenu"))
				{
					if (ImGui::MenuItem("Create Entity"))
					{
						engine.Get().Registry()->CreateEntity();
					}
					if (ImGui::MenuItem("Create Mesh"))
					{
						Entity mesh = engine.Get().Registry()->CreateEntity();
						engine.Get().Registry()->AddComponent(mesh, ENGINE::ECS::TransformComponent());
						engine.Get().Registry()->AddComponent(mesh, ENGINE::ECS::MeshComponent(engine.Get().AssetManager()->GetIdFromName("cube.obj")));
						engine.Get().Registry()->AddComponent(mesh, ENGINE::ECS::MaterialComponent(engine.Get().AssetManager()->GetIdFromName("default.hlsl"), engine.Get().AssetManager()->GetIdFromName("White.png")));
					}

					ImGui::EndPopup();
				}

				for (const auto& item : entities)
				{
					ImGui::PushID(item->id);     // or index
					//ImGui::Text("%s", item->name.c_str());
					if (ImGui::MenuItem(item->name.c_str())) {
						selectedEntity = item->id;
					}

					if (ImGui::BeginPopupContextItem("EntityContextMenu"))
					{
						selectedEntity = item->id;
						if (ImGui::MenuItem("Delete Entity"))
						{
							engine.Get().Registry()->DestroyEntity(selectedEntity);
						}
						ImGui::EndPopup();
					}

					ImGui::PopID();
				}
				ImGui::End();
			}


			if (ImGui::Begin("Inspector")) {
				// Rendering Components
				ComponentUIDrawInput componentDrawInput = { selectedEntity, engine.Get().Registry(), engine.Get().AssetManager(), textInput, &floatInput, float2Input, float3Input, float4Input};
				std::vector<ComponentType> components = engine.Get().Registry()->GetAllComponentTypesOfEntity(selectedEntity);

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
							component.addFunc(engine.Get().Registry(), selectedEntity);
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
						engine.Get().AssetManager()->LoadAsset(textInput);
						ZeroMemory(textInput, ARRAYSIZE(textInput));
					}

					ImGui::EndPopup();
				}

				for (auto& asset : engine.Get().AssetManager()->GetLoadedAssets()) {
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
}