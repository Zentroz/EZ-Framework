#pragma once

#include"../Math.h"

namespace EZ {
	struct Transform {
	public:
		float3 GetPosition();
		void SetPosition(float3 position);
		void Translate(float3 translation);

		quaternion GetRotation();
		void SetRotation(quaternion rotation);

		float3 GetScale();
		void SetScale(float3);

	private:
		void* m_Internal = nullptr;
	};
}