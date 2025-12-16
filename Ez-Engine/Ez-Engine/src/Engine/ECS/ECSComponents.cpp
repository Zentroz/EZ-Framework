#include"ECSComponents.h"

#include<BulletCollision/CollisionShapes/btEmptyShape.h>
#include<BulletCollision/CollisionShapes/btBoxShape.h>
#include<BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include<BulletCollision/CollisionShapes/btCapsuleShape.h>
#include<BulletCollision/CollisionShapes/btCylinderShape.h>
#include<BulletDynamics/Dynamics/btRigidBody.h>
#include<LinearMath/btDefaultMotionState.h>

namespace ENGINE {
	namespace ECS {

		TransformComponent::TransformComponent() {
			position = float3::zero();
			scale = float3::one();
			rotation = quaternion::Identity();
		}

		TransformComponent::TransformComponent(float3 position, float3 scale, quaternion rotation) : position(position), scale(scale), rotation(rotation) {}

		void TransformComponent::AddPosition(float3 delta) {
			position = position + delta;
		}

		MeshComponent::MeshComponent() : assetId("") {}
		MeshComponent::MeshComponent(EUID assetId) : assetId(assetId) {}

		MaterialComponent::MaterialComponent()
			: shaderAssetId(""), textureAssetId(""), baseColor(float4::one()) {
			baseColor = float4::one();
			baseColorInput[0] = baseColor.x;
			baseColorInput[1] = baseColor.y;
			baseColorInput[2] = baseColor.z;
			baseColorInput[3] = baseColor.w;
		}

		MaterialComponent::MaterialComponent(EUID shaderAssetId, EUID textureAssetId, float4 baseColor)
			: shaderAssetId(shaderAssetId), textureAssetId(textureAssetId), baseColor(baseColor) {
			baseColorInput[0] = baseColor.x;
			baseColorInput[1] = baseColor.y;
			baseColorInput[2] = baseColor.z;
			baseColorInput[3] = baseColor.w;
		}

		void MaterialComponent::SetFloat(std::string name, float value) {
			// To be implmented
		}

		btCollisionShape* ColliderData::GetBulletCollisionShape() { return bulletCollider; }
		bool ColliderData::IsStatic() { return isStatic; }

		StaticPlaneColliderData::StaticPlaneColliderData() : ColliderData(offset) {
			btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
			bulletCollider = static_cast<btCollisionShape*>(groundShape);
			isStatic = true;
		}

		ColliderType StaticPlaneColliderData::GetType() { return ColliderType::Plane; }

		BoxColliderData::BoxColliderData(float3 offset, float3 halfExtents) : ColliderData(offset), halfExtents(halfExtents) {
			btBoxShape* box = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
			bulletCollider = static_cast<btCollisionShape*>(box);
		}

		ColliderType BoxColliderData::GetType() { return ColliderType::Box; }

		CapsuleColliderData::CapsuleColliderData(float3 offset, float radius, float height) : ColliderData(offset), radius(radius) {
			btCapsuleShape* shape = new btCapsuleShape(radius, height);
			bulletCollider = static_cast<btCollisionShape*>(shape);
		}
		ColliderType CapsuleColliderData::GetType() { return ColliderType::Capsule; }

		CylinderColliderData::CylinderColliderData(float3 offset, float3 halfExtents) : ColliderData(offset), halfExtents(halfExtents) {
			btCylinderShape* shape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
			bulletCollider = static_cast<btCollisionShape*>(shape);
		}
		ColliderType CylinderColliderData::GetType() { return ColliderType::Cylinder; }

		ColliderComponent::ColliderComponent(ColliderData* data) : data(data) {
			colliderType = data->GetType();
		}

		RigidBodyComponent::RigidBodyComponent(float mass) : mass(mass) {
			// Cube
			btEmptyShape* shape = new btEmptyShape();

			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(0, 0, 0));

			btDefaultMotionState* motion = new btDefaultMotionState(startTransform);

			btVector3 inertia(0, 0, 0);

			btRigidBody::btRigidBodyConstructionInfo bodyCI(
				mass,
				motion,
				shape,
				inertia
			);

			bulletRb = new btRigidBody(bodyCI);
		}

		void RigidBodyComponent::SetCollisionShape(ColliderComponent& collider) {
			btVector3 inertia(0, 0, 0);

			if (!collider.data->IsStatic() && mass > 0.0f) {
				collider.data->GetBulletCollisionShape()->calculateLocalInertia(mass, inertia);
				bulletRb->activate(true);
			}
			else mass = 0;

			bulletRb->setCollisionShape(collider.data->GetBulletCollisionShape());
			bulletRb->setMassProps(mass, inertia);
			bulletRb->updateInertiaTensor();
		}

		void RigidBodyComponent::SetPosition(float3 position) {
			btTransform transform;
			bulletRb->getMotionState()->getWorldTransform(transform);

			transform.setOrigin(btVector3(position.x, position.y, position.z));

			bulletRb->setCenterOfMassTransform(transform);
		}

		void RigidBodyComponent::SetRotation(quaternion rotation) {
			btTransform transform;
			bulletRb->getMotionState()->getWorldTransform(transform);

			transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

			bulletRb->setCenterOfMassTransform(transform);
		}

		KinematicBody::KinematicBody() {
			float mass = 0;

			btEmptyShape* shape = new btEmptyShape();

			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(0, 0, 0));

			btDefaultMotionState* motion = new btDefaultMotionState(startTransform);

			btVector3 inertia(0, 0, 0);

			btRigidBody::btRigidBodyConstructionInfo bodyCI(
				mass,
				motion,
				shape,
				inertia
			);

			bulletRb = new btRigidBody(bodyCI);

			bulletRb->setFlags(bulletRb->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			bulletRb->setActivationState(DISABLE_DEACTIVATION);
		}

		void KinematicBody::SetCollisionShape(ColliderComponent& collider) {
			float mass = 0;

			bulletRb->setCollisionShape(collider.data->GetBulletCollisionShape());
			bulletRb->updateInertiaTensor();
		}

		ParticleComponent::ParticleComponent(float3 startPosition, float3 veloctiy, float size, float lifeTime, float speed, bool applyGravity)
			: startPosition(startPosition), position(startPosition), veloctiy(veloctiy), size(size), startSize(size), lifeTime(lifeTime), currentLifeTime(0),
			speed(speed), startSpeed(speed), applyGravity(applyGravity) {
		}

		ScriptComponent::ScriptComponent(std::string nameSpace, std::string className) : className(className), nameSpace(nameSpace) {}

	}
}