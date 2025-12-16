#pragma once

#include<unordered_map>
#include<Engine/Engine.h>

#include"Editor/ComponentDrawFunctions.h"

class InspectorPanel{
public:
	InspectorPanel();

	void OnRenderImGui(EUID selectedEntity);

private:
	struct ComponentListItem {
		std::string name;
		std::function<void(ENGINE::ECS::Registry&, Ref<ENGINE::ECS::Entity>)> addFunc;
		std::function<void(EDITOR::ComponentUIDrawInput& input)> drawFunc;

		ComponentListItem() = default;
		ComponentListItem(std::string name,
			std::function<void(ENGINE::ECS::Registry&, Ref<ENGINE::ECS::Entity>)> addFunc,
			std::function<void(EDITOR::ComponentUIDrawInput& input)> drawFunc
		) : name(name), addFunc(addFunc), drawFunc(drawFunc) {
		}
	};

private:
	std::unordered_map<ComponentType, ComponentListItem> componentItems;

	// Input buffers
	char textInput[256] = "";
	float floatInput = 0;
	float float2Input[2] = { 0, 0 };
	float float3Input[3] = { 0, 0, 0 };
	float float4Input[4] = { 0, 0, 0, 0 };
};