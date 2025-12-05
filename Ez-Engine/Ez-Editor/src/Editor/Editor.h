#pragma once

#include<string>
#include<functional>

#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_dx11.h>
#include<imgui/backends/imgui_impl_win32.h>

#include"Engine/Engine.h"
#include"Engine/Assets/AssetManager.h"

#include"FileDialog.h"
#include"ComponentDrawFunctions.h"

namespace EDITOR {

	struct ComponentListItem {
		std::string name;
		std::function<void(ENGINE::ECS::Registry*, Entity)> addFunc;
		std::function<void(ComponentUIDrawInput& input)> drawFunc;

		ComponentListItem() = default;
		ComponentListItem(std::string name,
			std::function<void(ENGINE::ECS::Registry*, Entity)> addFunc,
			std::function<void(ComponentUIDrawInput& input)> drawFunc
		) : name(name), addFunc(addFunc), drawFunc(drawFunc) { }
	};

	class Editor {
	public:
		Editor() = default;

		void Init();
		void Shutdown();

		void Update();
		void RenderUI();

		// Temp
		ComponentListItem& GetComponentListItem(ComponentType type) {

		}

	private:
		ENGINE::Engine engine;

		// ImGui
		ImVec2 sceneScreenSize;
		bool isOpenMain;
		Entity selectedEntity;

		// Input buffers
		char textInput[256] = "";
		float floatInput = 0;
		float float2Input[2] = { 0, 0 };
		float float3Input[3] = { 0, 0, 0 };
		float float4Input[4] = { 0, 0, 0, 0 };

		// Temp
		std::vector<ENGINE::ECS::GameEntity*> entities;
		std::unordered_map<ComponentType, ComponentListItem> componentItems;
	};
}