#ifndef RENDER_ITEM_CLASS_H
#define RENDER_ITEM_CLASS_H

#include<DirectXMath.h>
#include"Resources.h"

struct RenderItem {
public:
	std::string meshPath;
	std::string shaderPath;
	DirectX::XMMATRIX model;

	RenderItem(std::string meshPath, std::string shaderPath, float3 position, float3 scale, quaternion rotation) : meshPath(meshPath), shaderPath(shaderPath) {
		model = DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		);
	}
};

#endif