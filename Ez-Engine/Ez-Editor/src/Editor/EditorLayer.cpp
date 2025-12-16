#include"Editor/EditorLayer.h"
#include"Editor/Core/FileDialog.h"

#include<filesystem>
#include<Engine/Serialization/Serializer.h>
#include<Engine/Assets/AssetImporter.h>

namespace fs = std::filesystem;

void EditorLayer::OnAttach() {
	selectedEntity = EUID("");
}

void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate() {
	ENGINE::Engine::Get().sceneViewportSize = float2(sceneScreenSize.x, sceneScreenSize.y);
}

void EditorLayer::OnRenderImGui() {
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiDockNodeFlags_NoUndocking;

	//bool openLoadPopup = ENGINE::Engine::Get().Project().IsLoaded();

	if (ImGui::BeginPopupModal("Project"))
	{
		if (ImGui::Button("Create", ImVec2(120, 0)))
		{
			std::wstring path = OpenFolderDialog();
			if (!path.empty()) {
				fs::path dir(path);

				if (ENGINE::Engine::Get().Project().CreateProject("Test Project", dir)) {
					fs::path assetsPath = ENGINE::Engine::Get().Project().GetAssetDirectory();
					assetBrowser.SetDirectory(assetsPath);
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Open", ImVec2(120, 0)))
		{
			std::wstring path = OpenFolderDialog();
			if (!path.empty()) {
				fs::path dir(path);

				if (ENGINE::Engine::Get().Project().OpenProject(fs::path(path))) {
					fs::path assetsPath = ENGINE::Engine::Get().Project().GetAssetDirectory();
					assetBrowser.SetDirectory(assetsPath);
					ImGui::CloseCurrentPopup();
				}
			}
		}

		ImGui::EndPopup();
		//OutputDebugStringA("Popup\n");
	}
	if (!ENGINE::Engine::Get().Project().IsLoaded()) ImGui::OpenPopup("Project");

	if (ImGui::Begin("Editor", nullptr, windowFlags)) {
		RenderUI();
		assetBrowser.OnRenderImGui();
		inspectorPanel.OnRenderImGui(selectedEntity);
	}

	ImGui::End();
}

void EditorLayer::RenderUI() {

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save")) {
				ENGINE::Engine::Get().GetEngine()->SaveProject();
			}

			if (ImGui::MenuItem("Load")) {
				std::wstring file = OpenFileDialog("", ENGINE::Engine::Get().Backend().GetHwnd());
				ENGINE::Engine::Get().GetEngine()->LoadProject(std::string(file.begin(), file.end()));
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Asset")) {

			if (ImGui::MenuItem("Load Asset")) {
				std::wstring filepath = OpenFileDialog("", ENGINE::Engine::Get().Backend().GetHwnd());
				std::filesystem::path path(filepath);

				ENGINE::Engine::Get().AssetManager().Load(path);
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
		ID3D11ShaderResourceView* srv = ENGINE::Engine::Get().Backend().GetCustomRenderTarget()->GetSRV();
		ImGui::Image((ImTextureID)srv, ImVec2(sceneScreenSize.x, sceneScreenSize.y));
		//ImGui::Text("No Camera To Render!");
		ImGui::End();
	}

	if (ImGui::Begin("Hierarchy")) {
		if (ImGui::BeginPopupContextWindow("HierarchyContextMenu"))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				ENGINE::Engine::Get().Registry().CreateEntity("");
			}

			ImGui::EndPopup();
		}

		ENGINE::Engine::Get().Registry().Each([this](EUID id)
			{
				Entity entity = ENGINE::Engine::Get().Registry().GetEntity(id)->get();

				ImGui::PushID(entity.ruid);

				if (ImGui::MenuItem(entity.name.c_str())) {
					selectedEntity = entity.uid;
				}

				if (ImGui::BeginPopupContextItem("EntityContextMenu"))
				{
					selectedEntity = entity.uid;
					if (ImGui::MenuItem("Delete Entity"))
					{
						ENGINE::Engine::Get().Registry().DestroyEntity(entity.uid);
						selectedEntity = EUID("");
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
		);
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
} 