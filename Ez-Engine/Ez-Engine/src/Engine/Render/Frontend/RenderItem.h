#ifndef RENDER_ITEM_CLASS_H
#define RENDER_ITEM_CLASS_H

#include<DirectXMath.h>
#include"ECSComponents.h"

namespace ENGINE {
	namespace RENDERER {

		struct RenderItem {
		public:
			ECS::MaterialComponent material;
			ECS::MeshComponent mesh;
			DirectX::XMMATRIX model;

			RenderItem(ECS::TransformComponent transform, ECS::MaterialComponent material, ECS::MeshComponent mesh) : material(material), mesh(mesh) {
				model = DirectX::XMMatrixTranspose(
					DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z) *
					DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w)) *
					DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z)
				);
			}
		};

	}
}

#endif