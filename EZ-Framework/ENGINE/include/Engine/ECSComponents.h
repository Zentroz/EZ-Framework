#ifndef ECSCOMPONENT_CLASS_H
#define ECSCOMPONENT_CLASS_H

// Bullet
#include"BulletCollision/CollisionShapes/btEmptyShape.h"
#include"BulletCollision/CollisionShapes/btBoxShape.h"
#include"BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include"BulletDynamics/Dynamics/btRigidBody.h"
#include"LinearMath/btDefaultMotionState.h"

#include"Core/Math.h"

#include"ECSData.h"	
#include"sol/sol.hpp"

struct IComponent {
	Entity entity;

	static void RegisterWrapper(sol::state& m_lua) {} // Add and call this func for registering in Lua
};

struct TransformComponent : public IComponent {
	float3 position;
	float3 scale;
	quaternion rotation;

	TransformComponent() {
		position = float3::zero();
		scale = float3::one();
		rotation = quaternion::Identity();
	}

	TransformComponent(float3 position, float3 scale, quaternion rotation) : position(position), scale(scale), rotation(rotation) {}

	void AddPosition(float3 delta) {
		position = position + delta;
	}

	static void RegisterWrapper(sol::state* lua) {
		lua->new_usertype<TransformComponent>("Transform",
			sol::constructors<TransformComponent(), TransformComponent(float3, float3, quaternion)>(),
			"position", &TransformComponent::position,
			"scale", &TransformComponent::scale,
			"rotation", &TransformComponent::rotation,
			"AddPosition", &TransformComponent::AddPosition
		);
	}
};

struct MeshComponent : public IComponent {
	std::string path;

	MeshComponent(std::string path) : path(path) {}
};

struct MaterialComponent : public IComponent {
	std::string shaderPath;
	std::string mainTexturePath;
	float4 baseColor;

	MaterialComponent(std::string shaderPath, std::string mainTexturePath = "", float4 baseColor = float4::one()) : shaderPath(shaderPath), 
		mainTexturePath(mainTexturePath), baseColor(baseColor) {}

	void SetFloat(std::string name, float value) {
		// To be implmented
	}
};

enum ColliderType { Box, Cylinder, Capsule, Sphere };

struct ColliderData {
	float3 offset;

	virtual ~ColliderData() = default;
	virtual ColliderType GetType() = 0;

	btCollisionShape* GetBulletCollisionShape() { return bulletCollider; }

protected:
	ColliderData(float3 offset) : offset(offset) {}
	btCollisionShape* bulletCollider = nullptr;
};

struct BoxColliderData : public ColliderData {
	float3 halfExtents;

	BoxColliderData(float3 offset, float3 halfExtents) : ColliderData(offset), halfExtents(halfExtents) {
		btBoxShape* box = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));

		float mass = 1.0f;
		btVector3 inertia(0, 0, 0);
		box->calculateLocalInertia(mass, inertia);

		bulletCollider = static_cast<btCollisionShape*>(box);
	}

	ColliderType GetType() override { return ColliderType::Box; }
};

struct CapsuleColliderData : public ColliderData {
	float3 halfExtents;

	CapsuleColliderData(float3 offset, float3 halfExtents) : ColliderData(offset), halfExtents(halfExtents) {}
	ColliderType GetType() override { return ColliderType::Capsule; }
};

struct ColliderComponent : public IComponent {
	ColliderType colliderType;
	ColliderData* data;

	ColliderComponent(ColliderData* data) : data(data) {
		colliderType = data->GetType();
	}
};

struct RigidBodyComponent : public IComponent {
	btRigidBody* bulletRb = nullptr;

	RigidBodyComponent() {
		// Cube
		btEmptyShape* shape = new btEmptyShape();

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 0, 0));

		btDefaultMotionState* motion = new btDefaultMotionState(startTransform);

		float mass = 1.0f;
		btVector3 inertia(0, 0, 0);

		btRigidBody::btRigidBodyConstructionInfo bodyCI(
			mass,
			motion,
			shape,
			inertia
		);

		bulletRb = new btRigidBody(bodyCI);
	}

	void SetCollisionShape(btCollisionShape* collisionShape) {
		float mass = bulletRb->getMass();
		btVector3 inertia(0, 0, 0);

		if (mass > 0.0f)
			collisionShape->calculateLocalInertia(mass, inertia);

		bulletRb->setCollisionShape(collisionShape);
		bulletRb->setMassProps(mass, inertia);
		bulletRb->updateInertiaTensor();

		bulletRb->activate(true);
	}

	void SetPosition(float3 position) {
		btTransform transform;
		bulletRb->getMotionState()->getWorldTransform(transform);

		transform.setOrigin(btVector3(position.x, position.y, position.z));

		bulletRb->setCenterOfMassTransform(transform);
	}

	void SetRotation(quaternion rotation) {
		btTransform transform;
		bulletRb->getMotionState()->getWorldTransform(transform);

		transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

		bulletRb->setCenterOfMassTransform(transform);
	}
};

#endif