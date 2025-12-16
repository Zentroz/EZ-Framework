#include"PhysicsManager.h"

#include<BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include<BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include<BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include<BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include<BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include<BulletCollision/CollisionShapes/btBoxShape.h>
#include<BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include<LinearMath/btDefaultMotionState.h>

namespace ENGINE {
	namespace PHYSICS {

		PhysicsManager::PhysicsManager() {
			broadphase = new btDbvtBroadphase();
			config = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(config);
			solver = new btSequentialImpulseConstraintSolver();

			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
			world->setGravity(btVector3(0, -9.8f, 0));

			EventBus::Subscribe<ComponentAddedEvent<ECS::ColliderComponent>>(
				[this](const ComponentAddedEvent<ECS::ColliderComponent>& data) {
					OnColliderAdded(data);
				}
			);
			EventBus::Subscribe<ComponentAddedEvent<ECS::RigidBodyComponent>>(
				[this](const ComponentAddedEvent<ECS::RigidBodyComponent>& data) {
					OnRigidbodyAdded(data);
				}
			);
			EventBus::Subscribe<ComponentAddedEvent<ECS::KinematicBody>>(
				[this](const ComponentAddedEvent<ECS::KinematicBody>& data) {
					OnKinematicbodyAdded(data);
				}
			);
		}

		void PhysicsManager::OnColliderAdded(const ComponentAddedEvent<ECS::ColliderComponent>& data) {
			if (static_cast<ECS::Registry*>(data.registry)->Has<ECS::RigidBodyComponent>(data.entity)) {
				ECS::RigidBodyComponent& rb = static_cast<ECS::Registry*>(data.registry)->GetComponent<ECS::RigidBodyComponent>(data.entity);

				rb.SetCollisionShape(data.component);
			}
			else if (static_cast<ECS::Registry*>(data.registry)->Has<ECS::KinematicBody>(data.entity)) {
				ECS::KinematicBody& kb = static_cast<ECS::Registry*>(data.registry)->GetComponent<ECS::KinematicBody>(data.entity);

				kb.SetCollisionShape(data.component);
			}
		}

		void PhysicsManager::OnRigidbodyAdded(const ComponentAddedEvent<ECS::RigidBodyComponent>& data) {
			if (static_cast<ECS::Registry*>(data.registry)->Has<ECS::ColliderComponent>(data.entity)) {
				ECS::ColliderComponent& col = static_cast<ECS::Registry*>(data.registry)->GetComponent<ECS::ColliderComponent>(data.entity);

				ECS::RigidBodyComponent& rb = ((ECS::Registry*)data.registry)->GetComponent<ECS::RigidBodyComponent>(data.entity);
				rb.SetCollisionShape(col);

				world->addRigidBody(rb.bulletRb);
			}
		}
		void PhysicsManager::OnKinematicbodyAdded(const ComponentAddedEvent<ECS::KinematicBody>& data) {
			if (static_cast<ECS::Registry*>(data.registry)->Has<ECS::ColliderComponent>(data.entity)) {
				ECS::ColliderComponent& col = static_cast<ECS::Registry*>(data.registry)->GetComponent<ECS::ColliderComponent>(data.entity);

				ECS::KinematicBody& kb = ((ECS::Registry*)data.registry)->GetComponent<ECS::KinematicBody>(data.entity);
				kb.SetCollisionShape(col);

				world->addRigidBody(kb.bulletRb);
			}
		}

		void PhysicsManager::Update(ECS::Registry* registry) {
			float dt = GameTime::deltaTime > 0.05f ? 0.05f : GameTime::deltaTime;

			std::vector<uint64_t> physicsEntities = registry->view().Has<ECS::TransformComponent, ECS::RigidBodyComponent, ECS::ColliderComponent>().List();

			for (uint64_t e : physicsEntities) {
				ECS::TransformComponent t = registry->GetComponent<ECS::TransformComponent>(e);
				ECS::RigidBodyComponent r = registry->GetComponent<ECS::RigidBodyComponent>(e);
				ECS::ColliderComponent c = registry->GetComponent<ECS::ColliderComponent>(e);

				btTransform cubeTransform(btQuaternion(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w), btVector3(t.position.x, t.position.y, t.position.z));

				r.SetPosition(t.position);
			}

			simTimer += dt;

			world->stepSimulation(btScalar(1.0f) / btScalar(60.0f), btScalar(1.0f) / btScalar(60.0f));

			/*
			while (simTimer >= simTimeInterval) {
				simTimer -= simTimeInterval;
				OutputDebugStringA("Simulating...\n");
				OutputDebugStringA(("Delta: " + std::to_string(dt) + ", SimTimer : " + std::to_string(simTimer) + ", SimInterval: " + std::to_string(simTimeInterval) + "\n").c_str());
			}*/

			for (uint64_t e : physicsEntities) {
				ECS::TransformComponent& t = registry->GetComponent<ECS::TransformComponent>(e);
				ECS::RigidBodyComponent& r = registry->GetComponent<ECS::RigidBodyComponent>(e);
				ECS::ColliderComponent& c = registry->GetComponent<ECS::ColliderComponent>(e);

				btTransform transform;
				r.bulletRb->getMotionState()->getWorldTransform(transform);

				btVector3 btPos = transform.getOrigin();
				btQuaternion btQuat = transform.getRotation();

				t.position = float3(btPos.x(), btPos.y(), btPos.z());
				t.rotation = quaternion(btQuat.x(), btQuat.y(), btQuat.z(), btQuat.w());
			}
		}

	}
}