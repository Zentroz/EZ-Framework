#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<DirectXMath.h>

#include"Engine/Core/Math.h"
#include"Engine/ECS/Registry.h"

using namespace DirectX;

namespace ENGINE {
	namespace RENDERER {

		class Camera {
		public:
			Camera();

			uint32_t GetAssignedEntity() { return m_entity; }
			void AssignEntity(uint32_t entity, ECS::Registry* registry);

			XMMATRIX GetViewMatrix() const;
			XMMATRIX GetProjectionMatrix() const;

			MATH::float3 GetPosition() { return position; }

			void SetAspectRatio(float ratio) { this->aspectRatio = ratio; }

			void Update(ECS::Registry* registry);

		private:
			float aspectRatio = 1.0f;
			uint32_t m_entity;
			MATH::float3 position;
			MATH::quaternion rotation;
		};

	}
}

#endif