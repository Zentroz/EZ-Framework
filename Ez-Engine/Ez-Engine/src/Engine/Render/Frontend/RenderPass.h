#ifndef RENDERPASS_CLASS_H
#define RENDERPASS_CLASS_H

#include<d3d11.h>

#include"Engine/ECS/Registry.h"
#include"Engine/Assets/AssetManager.h"
#include"Engine/Render/RenderContext.h"
#include"Engine/Render/Frontend/Camera.h"
#include"Engine/Render/Frontend/ResourceManager.h"
#include"Engine/Render/PipelineStateManager.h"

namespace ENGINE {
	namespace RENDERER {

		struct RenderPassContext {
			Camera* camera = nullptr;
			ECS::Registry* registry = nullptr;
			RenderContext* ctx = nullptr;
			ResourceManager* resources = nullptr;
			ASSET::AssetManager* assets = nullptr;
			PipelineStateManager* pipelineStates = nullptr;
			ID3D11Buffer* globalBuffer = nullptr;

			RenderPassContext() = default;
			RenderPassContext(Camera* camera, ECS::Registry* registry, RenderContext* ctx, ASSET::AssetManager* assets, ResourceManager* resources, PipelineStateManager* pipelineStates, ID3D11Buffer* globalBuffer)
				: camera(camera), registry(registry), ctx(ctx), assets(assets), resources(resources), pipelineStates(pipelineStates), globalBuffer(globalBuffer) {
			}
		};

		class RenderPass {
		public:
			virtual ~RenderPass() = default;
			virtual void Execute() = 0;
		};

	}
}

#endif