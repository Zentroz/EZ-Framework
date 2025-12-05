#include"Renderer.h"

namespace ENGINE {
	namespace RENDERER {

		Renderer::Renderer() {}

		void Renderer::Init(RendererInitData initData) {
			this->ctx = initData.ctx;

			depthStencilView = initData.resourceManager->CreateDepthStencilBuffer(initData.width, initData.height);

			pipelineStateManager.Init(initData.device);

			CreateGlobalBuffer(initData.device);

			renderPassContext = RenderPassContext(GetCamera(), initData.registry, ctx, initData.assetManger, initData.resourceManager, &pipelineStateManager, globalBuffer);

			AddRenderPass(new SceneRenderPass(renderPassContext, initData.device));
			//AddRenderPass(new DebugRenderPass(registry, ctx, &resources, GetCamera(), &pipelineStateManager, device, globalBuffer));
		}

		void Renderer::Shutdown() {}


		void Renderer::CreateGlobalBuffer(ID3D11Device* device) {
			HRESULT hr;

			D3D11_BUFFER_DESC cbd = {};
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.ByteWidth = sizeof(GlobalConstantBuffer);
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			hr = device->CreateBuffer(&cbd, nullptr, &globalBuffer);

			CHECK_DXHR(hr, "Failed to create global buffer.");
		}

		void Renderer::SetGlobalBufferData() {
			GlobalConstantBuffer cbData;

			cbData.view = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
			cbData.projection = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());
			float3 camPos = camera.GetPosition();
			cbData.cameraPosition = float4(camPos.x, camPos.y, camPos.z, 0);
			cbData.lightDirection = (quaternion::FromAxisAngle(float3(0, 1, 0), 30 * Deg2Rad) * quaternion::FromAxisAngle(float3(1, 0, 0), 65 * Deg2Rad)).forward() * -1;
			cbData.timeData = float4(GameTime::time, GameTime::deltaTime, 0, 0);

			ctx->UpdateMappedSubresource(globalBuffer, &cbData, sizeof(cbData));
		}

		void Renderer::InitRender(RenderTarget* mainRenderTarget) {
			ctx->ClearRenderTarget(mainRenderTarget->GetRTV(), float4(0.15f, 0.55f, 0.75f, 1.0f));
			ctx->ClearDepthStencilView(depthStencilView.Get());

			ctx->SetRenderTarget(mainRenderTarget->GetRTV(), depthStencilView.Get());
			ctx->SetViewport();
		}

		void Renderer::Render() {
			SetGlobalBufferData();

			for (RenderPass* pass : renderPasses) {
				pass->Execute();
			}
		}

		void Renderer::EndRender() {
			ctx->SetRenderTarget(nullptr, nullptr);
		}

		void Renderer::BindLinearSampler() {
			ctx->SetSamplerState(0, pipelineStateManager.linerSampler.GetAddressOf());
		}

	}
}