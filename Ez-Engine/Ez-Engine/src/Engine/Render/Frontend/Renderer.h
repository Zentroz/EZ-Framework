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
			void CreateGlobalBuffer(ID3D11Device* device);
			void SetGlobalBufferData();
			void InitRender(RenderTarget* mainRenderTarget);
			void Render();
			void EndRender();

			void BindLinearSampler();

			void AddRenderPass(RenderPass* renderPass) { renderPasses.push_back(renderPass); }
	
			Camera* GetCamera() noexcept { return &camera; }
		private:
			RenderContext* ctx = nullptr;
			PipelineStateManager pipelineStateManager;
			Camera camera;
			RenderPassContext renderPassContext;

			std::vector<RenderPass*> renderPasses = {};

			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

			ID3D11Buffer* globalBuffer = nullptr;
			struct GlobalConstantBuffer {
				DirectX::XMMATRIX view;
				DirectX::XMMATRIX projection;
				float4 lightDirection;
				float4 cameraPosition;
				float4 timeData;
			};
		};

	}
}


#endif