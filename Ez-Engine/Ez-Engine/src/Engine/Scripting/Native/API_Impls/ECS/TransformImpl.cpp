#include<EzAPI/ECS/Components.h>
#include"Engine/ECS/ECSComponents.h"

namespace EZ {
	EZ::float3 Transform::GetPosition() {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		return { t->position.x, t->position.y, t->position.z };
	}
	void Transform::SetPosition(EZ::float3 position) {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		t->position = MATH::float3(position.x, position.y, position.z);
	}
	void Transform::Translate(EZ::float3 position) {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		t->position += MATH::float3(position.x, position.y, position.z);
	}
	EZ::float3 Transform::GetScale() {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		return { t->scale.x, t->scale.y, t->scale.z };
	}
	void Transform::SetScale(EZ::float3 position) {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		t->scale = MATH::float3(position.x, position.y, position.z);
	}
	EZ::quaternion Transform::GetRotation() {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		return { t->rotation.x, t->rotation.y, t->rotation.z, t->rotation.w };
	}
	void Transform::SetRotation(EZ::quaternion rotation) {
		ENGINE::ECS::TransformComponent* t = static_cast<ENGINE::ECS::TransformComponent*>(m_Internal);
		t->rotation = MATH::quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
	}
}