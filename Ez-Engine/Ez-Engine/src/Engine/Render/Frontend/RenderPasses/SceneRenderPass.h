#ifndef SCENE_RENDERPASS_H
#define SCENE_RENDERPASS_H

#include<DirectXMath.h>

#include"Engine/Core/GameTime.h"
#include"Engine/ECS/Registry.h"
#include"Engine/Render/Frontend/Camera.h"
#include"Engine/Render/PipelineStateManager.h"
#include"Engine/Render/Frontend/ResourceManager.h"
#include"Engine/Render/Frontend/RenderPass.h"

namespace ENGINE {
	namespace RENDERER {

		class SceneRenderPass : public RenderPass {
		public:
			SceneRenderPass(const RenderPassContext& context, ID3D11Device* device)
				: ctx(context) {

				D3D11_BUFFER_DESC pOD = {};
				pOD.Usage = D3D11_USAGE_DYNAMIC;
				pOD.ByteWidth = sizeof(PerObjectBuffer);
				pOD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				pOD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				HRESULT hr = device->CreateBuffer(&pOD, nullptr, &perObjectBuffer);

				CHECK_DXHR(hr, "Failed to create Per Object buffer.");
			}
			void Execute() override {
				ctx.ctx->SetDepthStencilState(ctx.pipelineStates->depthWrite);
				ctx.ctx->SetRasteriser(ctx.pipelineStates->solid);

				PerObjectBuffer perObjectData;
				EUID lastShader("");

				ctx.ctx->SetTopology();

				for (const RenderItem& item : ctx.registry->CreateRenderList()) {
					std::shared_ptr<Mesh> mesh = ctx.resources->Load<Mesh>(item.mesh.assetId);
					std::shared_ptr<Texture> texture = ctx.resources->Load<Texture>(item.material.textureAssetId);

					perObjectData.model = item.model;
					perObjectData.color = item.material.baseColor;

					ctx.ctx->UpdateMappedSubresource(perObjectBuffer, &perObjectData, sizeof(perObjectData));

					if (lastShader.isNull() || lastShader != item.material.shaderAssetId) {
						std::shared_ptr<ASSET::Asset> shaderAsset = ctx.assets->GetAsset(item.material.shaderAssetId);
						if (shaderAsset == nullptr) continue;
						std::shared_ptr<Shader> shader = ctx.resources->Load<Shader>(item.material.shaderAssetId);

						if (shader != nullptr) {
							ctx.ctx->SetInputLayout(shader->GetInputLayout());
							ctx.ctx->SetShader(shader.get());
							lastShader = item.material.shaderAssetId;
						}
					}

					ID3D11ShaderResourceView* textureSRV = nullptr;

					if (texture != nullptr) {
						textureSRV = texture->GetSRV();
						ctx.ctx->SetSamplerState(0, ctx.pipelineStates->linerSampler.GetAddressOf());
						ctx.ctx->SetPSShaderResource(0, &textureSRV);
					}

					ctx.ctx->SetVSConstantBuffer(ctx.frameBuffer->GetBuffer(), 0);
					ctx.ctx->SetPSConstantBuffer(ctx.frameBuffer->GetBuffer(), 0);
					ctx.ctx->SetVSConstantBuffer(perObjectBuffer, 1);
					ctx.ctx->SetPSConstantBuffer(perObjectBuffer, 1);

					if (mesh != nullptr) {
						ctx.ctx->SetMesh(mesh.get());
						ctx.ctx->DrawIndexed(mesh->GetIndexCount());
					}
				}

				// Particles
				/*
				std::shared_ptr<ASSET::Asset> shaderAsset = ctx.assets->GetAsset(
					ctx.assets->GetIdFromName("particle.hlsl")
				);

				std::shared_ptr<Shader> shader = ctx.resources->Load<Shader>(shaderAsset);

				if (shader != nullptr) {
					ctx.ctx->SetInputLayout(shader->GetInputLayout());
					ctx.ctx->SetShader(shader.get());
				}

				ctx.ctx->SetVSConstantBuffer(ctx.globalBuffer, 0);
				ctx.ctx->SetPSConstantBuffer(ctx.globalBuffer, 0);
				ctx.ctx->SetVSConstantBuffer(perObjectBuffer, 1);
				ctx.ctx->SetPSConstantBuffer(perObjectBuffer, 1);

				for (Entity e : ctx.registry->view().Has<ECS::ParticleComponent>().List()) {
					ECS::ParticleComponent& particle = ctx.registry->GetComponent<ECS::ParticleComponent>(e);

					perObjectData.model = DirectX::XMMatrixTranspose(
						DirectX::XMMatrixScaling(particle.size, particle.size, particle.size) *
						DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(0, 0, 0, 1)) *
						DirectX::XMMatrixTranslation(particle.position.x, particle.position.y, particle.position.z)
					);
					perObjectData.color = MATH::float4(1, 0, 0, 1);
					ctx.ctx->UpdateMappedSubresource(perObjectBuffer, &perObjectData, sizeof(perObjectData));

					std::shared_ptr<ASSET::Asset> meshAsset = ctx.assets->GetAsset(
						ctx.assets->GetIdFromName("cube.obj")
					);
					std::shared_ptr<Mesh> mesh = ctx.resources->Load<Mesh>(meshAsset);

					if (mesh != nullptr) {
						ctx.ctx->SetMesh(mesh.get());
					}

					ctx.ctx->DrawIndexed(mesh->GetIndexCount());
				}
				*/
			}

		private:
			const RenderPassContext& ctx;

			ID3D11Buffer* perObjectBuffer = nullptr;
			struct PerObjectBuffer {
				DirectX::XMMATRIX  model;
				MATH::float4 color;
			};
		};

	}
}

#endif