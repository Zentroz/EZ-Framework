#ifndef COMPONENT_DRAW_FUNCTIONS_H
#define COMPONENT_DRAW_FUNCTIONS_H

#include<imgui/imgui.h>

#include"Engine/ECS/Registry.h"
#include"Engine/Assets/AssetManager.h"
#include"Engine/ECS/ECSComponents.h"
#include"Engine/Core/Logger.h"

using namespace ENGINE::ECS;
using namespace ENGINE::ASSET;

namespace EDITOR {
	struct ComponentUIDrawInput {
		Entity entity;
		Registry* registry;
		AssetManager* assetManager;

		// Input Buffers
		char* textBuffer;
		float* floatBuffer;
		float* float2Buffer;
		float* float3Buffer;
		float* float4Buffer;
	};

	void DrawTransform(ComponentUIDrawInput& input);
	void DrawMeshComponent(ComponentUIDrawInput& input);
	void DrawMaterialComponent(ComponentUIDrawInput& input);
}


#endif