#pragma once

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_win32.h>
#include<imgui/backends/imgui_impl_dx11.h>

#include<Engine/Engine.h>
#include<Engine/Layer/Layer.h>
#include"Editor/ComponentDrawFunctions.h"

class EditorLayer : public Layer {
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRenderImGui() override;

	void RenderUI();

private:
	struct ComponentListItem {
		std::string name;
		std::function<void(ENGINE::ECS::Registry*, Entity)> addFunc;
		std::function<void(EDITOR::ComponentUIDrawInput& input)> drawFunc;

		ComponentListItem() = default;
		ComponentListItem(std::string name,
			std::function<void(ENGINE::ECS::Registry*, Entity)> addFunc,
			std::function<void(EDITOR::ComponentUIDrawInput& input)> drawFunc
		) : name(name), addFunc(addFunc), drawFunc(drawFunc) {
		}
	};

private:
	ImVec2 sceneScreenSize;
	Entity selectedEntity;

	std::vector<ENGINE::ECS::GameEntity*> entities;
	std::unordered_map<ComponentType, ComponentListItem> componentItems;

	// Input buffers
	char textInput[256] = "";
	float floatInput = 0;
	float float2Input[2] = { 0, 0 };
	float float3Input[3] = { 0, 0, 0 };
	float float4Input[4] = { 0, 0, 0, 0 };
};