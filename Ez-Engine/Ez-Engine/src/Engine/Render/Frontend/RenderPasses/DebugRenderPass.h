#ifndef DEBUG_RENDERPASS_H
#define DEBUG_RENDERPASS_H

#include<DirectXMath.h>

#include"Engine/Core/GameTime.h"
#include"Engine/ECS/Registry.h"
#include"Engine/Render/Frontend/Camera.h"
#include"Engine/Render/PipelineStateManager.h"
#include"Engine/Render/Frontend/ResourceManager.h"
#include"Engine/Render/Frontend/RenderPass.h"

namespace ENGINE {
	namespace RENDERER {

		class DebugRenderPass : public RenderPass {
		public:
			DebugRenderPass(ECS::Registry* registry, RenderContext* ctx, ResourceManager* resources, Camera* camera, PipelineStateManager* pipelineStates, ID3D11Device* device, ID3D11Buffer* globalBuffer)
				: registry(registry), ctx(ctx), resources(resources), pipelineStates(pipelineStates), camera(camera), globalBuffer(globalBuffer) {

				D3D11_BUFFER_DESC pOD = {};
				pOD.Usage = D3D11_USAGE_DYNAMIC;
				pOD.ByteWidth = sizeof(PerObjectBuffer);
				pOD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				pOD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				HRESULT hr = device->CreateBuffer(&pOD, nullptr, &perObjectBuffer);

				CHECK_DXHR(hr, "Failed to create Per Object buffer.");
			}
			void Execute() override {
				/*
				ctx->SetDepthStencilState(pipelineStates->depthWrite);
				ctx->SetRasteriser(pipelineStates->wireframe);

				PerObjectBuffer perObjectData;

				ctx->SetTopology();

				std::shared_ptr<Shader> shader = resources->Load<Shader>("Assets/Shaders/collider.hlsl");

				ctx->SetInputLayout(shader->GetInputLayout());
				ctx->SetShader(shader.get());

				ctx->SetVSConstantBuffer(globalBuffer, 0);
				ctx->SetPSConstantBuffer(globalBuffer, 0);
				ctx->SetVSConstantBuffer(perObjectBuffer, 1);
				ctx->SetPSConstantBuffer(perObjectBuffer, 1);

				for (Entity e : registry->view().Has<ECS::ColliderComponent, ECS::TransformComponent>().List()) {
					ECS::ColliderComponent& c = registry->GetComponent<ECS::ColliderComponent>(e);
					ECS::TransformComponent& t = registry->GetComponent<ECS::TransformComponent>(e);

					std::shared_ptr<Mesh> mesh = nullptr;
					perObjectData.color = float4(0, 1, 0, 1);

					if (c.colliderType == ECS::ColliderType::Box) {
						mesh = resources->Load<Mesh>("Assets/Mesh/cube.obj");

						ECS::BoxColliderData* box = static_cast<ECS::BoxColliderData*>(c.data);

						float3 scale = box->halfExtents * 2;

						perObjectData.model = DirectX::XMMatrixTranspose(
							DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
							DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w)) *
							DirectX::XMMatrixTranslation(t.position.x, t.position.y, t.position.z)
						);
					}
					else if (c.colliderType == ECS::ColliderType::Cylinder) {
						mesh = resources->Load<Mesh>("Assets/Mesh/cylinder.obj");

						ECS::CylinderColliderData* cylinder = static_cast<ECS::CylinderColliderData*>(c.data);

						float3 scale = cylinder->halfExtents * 2;

						perObjectData.model = DirectX::XMMatrixTranspose(
							DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
							DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w)) *
							DirectX::XMMatrixTranslation(t.position.x, t.position.y, t.position.z)
						);
					}
					else {
						perObjectData.model = DirectX::XMMatrixTranspose(
							DirectX::XMMatrixScaling(t.scale.x, t.scale.y, t.scale.z) *
							DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w)) *
							DirectX::XMMatrixTranslation(t.position.x, t.position.y, t.position.z)
						);
					}

					if (mesh == nullptr) continue;

					ctx->UpdateMappedSubresource(perObjectBuffer, &perObjectData, sizeof(perObjectData));

					ctx->SetMesh(mesh.get());

					ctx->DrawIndexed(mesh->GetIndexCount());
				} */
			}

		private:
			Camera* camera = nullptr;

			ECS::Registry* registry = nullptr;
			RenderContext* ctx = nullptr;
			ResourceManager* resources = nullptr;
			PipelineStateManager* pipelineStates = nullptr;

			ID3D11Buffer* globalBuffer = nullptr;
			ID3D11Buffer* perObjectBuffer = nullptr;
			struct PerObjectBuffer {
				DirectX::XMMATRIX  model;
				float4 color;
			};
		};

	}
}

#endif