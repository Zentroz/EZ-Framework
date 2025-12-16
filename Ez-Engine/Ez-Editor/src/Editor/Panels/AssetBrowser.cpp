#include"Editor/Panels/AssetBrowser.h"
#include<Engine/Engine.h>
#include<fstream>
#include<imgui/imgui.h>

using namespace ENGINE;

namespace fs = std::filesystem;

AssetBrowser::AssetBrowser() {}

void AssetBrowser::SetDirectory(std::filesystem::path& path) {
	m_AssetsDir = path;
	m_CurrentDir = path;
}

void DrawFile(fs::path& path, float thumbnail) {
	/*
	auto meta = Engine::Get().AssetManager().GetMetaCache().Get(path);
	if (meta) {
		std::string fileName = path.filename().string();
		//ImGui::BeginGroup();
		ImGui::Button(fileName.c_str(), { thumbnail, thumbnail });
		//ImGui::Text(fileName.c_str());
		//ImGui::EndGroup();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set the payload data
			std::string assetPayload = meta->euid.Get();
			ImGui::SetDragDropPayload("ASSET_ID_PAYLOAD", assetPayload.c_str(), sizeof(char) * assetPayload.size() + 1);

			// Render a preview of the dragged item
			ImGui::Text(fileName.c_str());

			ImGui::EndDragDropSource();
		}
	}
	*/
}

void AssetBrowser::OnRenderImGui() {
	if (ImGui::Begin("Assets")) {

		if (ImGui::BeginPopupContextWindow("AssetsContextMenu"))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Folder")) {
					fs::path dirPath = m_CurrentDir / "New Folder";
					fs::create_directory(dirPath);
				}

				if (ImGui::MenuItem("Script")) {}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		float thumbnail = 64;
		float padding = 16;
		float cellSize = thumbnail + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = panelWidth / cellSize;

		for (auto& [euid, meta] : ENGINE::Engine::Get().AssetManager().GetMetaCache().GetMetaMap()) {
			fs::path metaPath = meta->metaPath;
			ImGui::Button(metaPath.stem().string().c_str(), { thumbnail, thumbnail });
			ImGui::NextColumn();
		}

		ImGui::Columns(columnCount, 0, false);

		ImGui::Columns(1);
		ImGui::End();
	}
}