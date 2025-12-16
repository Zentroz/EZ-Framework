#ifndef RENDERPASS_CLASS_H
#define RENDERPASS_CLASS_H

#include<d3d11.h>

#include"Engine/ECS/Registry.h"
#include"Engine/Assets/AssetManager.h"
#include"Engine/Render/RenderContext.h"
#include"Engine/Render/Frontend/Camera.h"
#include"Engine/Render/Frontend/ResourceManager.h"
#include"Engine/Render/PipelineStateManager.h"
#include"Engine/Render/Frontend/FrameDataBuffer.h"

namespace ENGINE {
	namespace RENDERER {

		struct RenderPassContext {
			ECS::Registry* registry = nullptr;
			RenderContext* ctx = nullptr;
			ResourceManager* resources = nullptr;
			ASSET::AssetManager* assets = nullptr;
			PipelineStateManager* pipelineStates = nullptr;
			FrameDataBuffer* frameBuffer = nullptr;

			RenderPassContext() = default;
			RenderPassContext(ECS::Registry* registry, RenderContext* ctx, ASSET::AssetManager* assets, ResourceManager* resources, PipelineStateManager* pipelineStates, FrameDataBuffer* frameBuffer)
				: registry(registry), ctx(ctx), assets(assets), resources(resources), pipelineStates(pipelineStates), frameBuffer(frameBuffer) {
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