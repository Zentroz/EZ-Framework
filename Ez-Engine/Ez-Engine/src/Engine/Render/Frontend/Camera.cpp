#include"Camera.h"

namespace ENGINE {
	namespace RENDERER {

		Camera::Camera() : position(MATH::float3(0, 10, -10)), rotation(MATH::quaternion::FromAxisAngle(MATH::float3::right(), 45)), m_entity(UINT32_MAX) {}

		XMMATRIX Camera::GetViewMatrix() const {
			MATH::float3 forward = rotation.forward();
			MATH::float3 up = rotation.up();

			XMMATRIX view = XMMatrixLookAtLH(
				XMVectorSet(position.x, position.y, position.z, 1.0f),
				XMVectorSet(position.x + forward.x, position.y + forward.y, position.z + forward.z, 1.0f),
				XMVectorSet(up.x, up.y, up.z, 0.0f)
			);

			return view;
		}

		XMMATRIX Camera::GetProjectionMatrix() const {
			XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), aspectRatio, 0.1f, 100.0f);

			return projection;
		}

		void Camera::Update(ECS::Registry* registry) {
			ECS::TransformComponent& t = registry->GetComponent<ECS::TransformComponent>(m_entity);

			position = t.position;
			rotation = t.rotation;
		}

		void Camera::AssignEntity(uint32_t entity, ECS::Registry* registry) {
			m_entity = entity;

			ECS::TransformComponent& t = registry->GetComponent<ECS::TransformComponent>(m_entity);

			t.position = position;
			t.rotation = rotation;
		}

	}
}