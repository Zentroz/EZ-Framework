#include"Engine/Camera.h"

Camera::Camera() : position(float3(0, 2, -10)), rotation(quaternion(0, 0, 0, 1)) {}

XMMATRIX Camera::GetViewMatrix() const {
	float3 forward = rotation.forward();
	float3 up = rotation.up();

	XMMATRIX view = XMMatrixLookAtLH(
		XMVectorSet(position.x, position.y, position.z, 1.0f),
		XMVectorSet(position.x + forward.x, position.y + forward.y, position.z + forward.z, 1.0f),
		XMVectorSet(up.x, up.y, up.z, 0.0f)
	);

	return view;
}

XMMATRIX Camera::GetProjectionMatrix(float aspectRatio) const {
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), aspectRatio, 0.1f, 100.0f);

	return projection;
}

void Camera::SetPosition(const float3& pos) {
	position = pos;
}