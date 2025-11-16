#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include"Core/Math.h"

#include<DirectXMath.h>

using namespace DirectX;

class Camera {
public:
	Camera();

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix(float aspectRatio) const;

	void SetPosition(const float3& pos);
	void SetPosition(const XMFLOAT3& pos);

	float3 GetPosition();

	void FlipForward(int to);
private:
	XMFLOAT3 position;
	XMFLOAT3 forward;
	XMFLOAT3 up;
};

#endif