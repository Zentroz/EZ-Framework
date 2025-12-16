#include"Renderer.h"

namespace ENGINE {
	namespace RENDERER {

		Renderer::Renderer() {}

		void Renderer::Init(RendererInitData initData) {
			this->ctx = initData.ctx;

			depthStencilView = initData.resourceManager->CreateDepthStencilBuffer(initData.width, initData.height);

			pipelineStateManager.Init(initData.device);

			frameDataBuffer.Init(initData.device);

			renderPassContext = RenderPassContext(initData.registry, ctx, initData.assetManger, initData.resourceManager, &pipelineStateManager, &frameDataBuffer);
			AddRenderPass(new SceneRenderPass(renderPassContext, initData.device));
			//AddRenderPass(new DebugRenderPass(registry, ctx, &resources, GetCamera(), &pipelineStateManager, device, globalBuffer));
		}

		void Renderer::Shutdown() {}

		void Renderer::InitRender(RenderTarget* mainRenderTarget) {
			ctx->ClearRenderTarget(mainRenderTarget->GetRTV(), float4(0.15f, 0.55f, 0.75f, 1.0f));
			ctx->ClearDepthStencilView(depthStencilView.Get());

			ctx->SetRenderTarget(mainRenderTarget->GetRTV(), depthStencilView.Get());
			ctx->SetViewport();
		}

		void Renderer::Render() {
			if (m_Camera == nullptr) return;

			FrameDataBuffer::FrameData cbData;
			cbData.view = DirectX::XMMatrixTranspose(m_Camera->GetViewMatrix());
			cbData.projection = DirectX::XMMatrixTranspose(m_Camera->GetProjectionMatrix());
			float3 camPos = m_Camera->position;
			cbData.cameraPosition = float4(camPos.x, camPos.y, camPos.z, 0);
			cbData.lightDirection = (quaternion::FromAxisAngle(float3(0, 1, 0), 30 * Deg2Rad) * quaternion::FromAxisAngle(float3(1, 0, 0), 65 * Deg2Rad)).forward() * -1;
			cbData.timeData = float4(GameTime::time, GameTime::deltaTime, 0, 0);

			frameDataBuffer.UpdateAndBindData(cbData, ctx);

			for (RenderPass* pass : renderPasses) {
				pass->Execute();
			}
		}

		void Renderer::EndRender() {
			ctx->SetRenderTarget(nullptr, nullptr);
		}

	}
}