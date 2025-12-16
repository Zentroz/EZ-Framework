#pragma once

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_win32.h>
#include<imgui/backends/imgui_impl_dx11.h>

#include<Engine/Engine.h>
#include<Engine/Layer/Layer.h>

#include"Editor/ComponentDrawFunctions.h"
#include"Editor/Panels/AssetBrowser.h"
#include"Editor/Panels/InspectorPanel.h"

class EditorLayer : public Layer {
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRenderImGui() override;

	void RenderUI();

private:
	// Panels
	AssetBrowser assetBrowser;
	InspectorPanel inspectorPanel;

	EUID selectedEntity;
	ImVec2 sceneScreenSize;

	std::vector<ENGINE::ECS::Entity> entities;

	// Input buffers
	char textInput[256] = "";
	float floatInput = 0;
	float float2Input[2] = { 0, 0 };
	float float3Input[3] = { 0, 0, 0 };
	float float4Input[4] = { 0, 0, 0, 0 };
};