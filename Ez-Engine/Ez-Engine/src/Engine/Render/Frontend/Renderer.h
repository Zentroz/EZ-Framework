#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include"Engine/Core/Math.h"
#include"Engine/Core/GameTime.h"
#include"Engine/Render/RenderTarget.h"
#include"Engine/Render/RenderContext.h"
#include"Engine/Render/PipelineStateManager.h"
#include"Engine/Render/Backend/EngineBackend.h"
#include"Engine/Render/Frontend/Camera.h"
#include"Engine/Render/Frontend/RenderItem.h"
#include"Engine/Render/Frontend/RenderPass.h"
#include"Engine/Render/Frontend/ResourceManager.h"
#include"Engine/Render/Frontend/RenderPasses/SceneRenderPass.h"
#include"Engine/Render/Frontend/RenderPasses/DebugRenderPass.h"
#include"Engine/Render/Frontend/FrameDataBuffer.h"

namespace ENGINE {
	namespace RENDERER {

		class Renderer {
		public:
			struct RendererInitData {
				ID3D11Device* device; RenderContext* ctx; ECS::Registry* registry; ASSET::AssetManager* assetManger; ResourceManager* resourceManager; int width; int height;
			};

			Renderer();

			void Init(RendererInitData initData);
			void Shutdown();

			void InitRender(RenderTarget* mainRenderTarget);
			void Render();
			void EndRender();

			void SetCamera(Camera* camera) { m_Camera = camera; }

			void AddRenderPass(RenderPass* renderPass) { renderPasses.push_back(renderPass); }

		private:
			PipelineStateManager pipelineStateManager;
			RenderPassContext renderPassContext;
			FrameDataBuffer frameDataBuffer;

			RenderContext* ctx = nullptr;
			Camera* m_Camera = nullptr;

			std::vector<RenderPass*> renderPasses = {};

			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		};

	}
}


#endif