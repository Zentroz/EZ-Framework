#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

// Bullet
#include"BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include"BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include"BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include"BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include"BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include"BulletCollision/CollisionShapes/btBoxShape.h"
#include"BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include"LinearMath/btDefaultMotionState.h"

#include"Registry.h"
#include"Core/GameTime.h"
#include"Engine/Collision.h"

class PhysicsManager {
public:
	PhysicsManager();

	void Update(Registry* registry);
	void OnColliderAdded(const ComponentAddedEvent<ColliderComponent>& data);

	void OnRigidbodyAdded(const ComponentAddedEvent<RigidBodyComponent>& data);

private:
	btBroadphaseInterface* broadphase = nullptr;
	btDefaultCollisionConfiguration* config = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	btRigidBody* groundBody = nullptr;
};

#endif