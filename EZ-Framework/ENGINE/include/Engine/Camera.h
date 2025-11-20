#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include"Core/Math.h"

#include<DirectXMath.h>

using namespace DirectX;

class Camera {
public:
	float3 position;
	quaternion rotation;

	Camera();

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix(float aspectRatio) const;

	void SetPosition(const float3& pos);
	void SetPosition(const XMFLOAT3& pos);
};

#endif