#include"Engine/Camera.h"

Camera::Camera() {
	position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

XMMATRIX Camera::GetViewMatrix() const {
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
	position = XMFLOAT3(pos.x, pos.y, pos.z);
}

void Camera::SetPosition(const XMFLOAT3& pos) {
	position = pos;
}

float3 Camera::GetPosition() {
	return float3(position.x, position.y, position.z);
}

void Camera::FlipForward(int to) {
	forward = XMFLOAT3(0, 0, to);
}