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
			MATH::float3 position;
			MATH::quaternion rotation;

			Camera();

			XMMATRIX GetViewMatrix() const;
			XMMATRIX GetProjectionMatrix() const;

			void SetAspectRatio(float ratio) { this->aspectRatio = ratio; }

		private:
			float aspectRatio = 1.0f;
		};

	}
}

#endif