#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include"Engine/Core/GameTime.h"
#include"Engine/Core/EventBus.h"
#include"Engine/ECS/Registry.h"

using namespace MATH;

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace ENGINE {
	namespace PHYSICS {

		class PhysicsManager {
		public:
			PhysicsManager();

			void Update(ECS::Registry* registry);
			void OnColliderAdded(const ComponentAddedEvent<ECS::ColliderComponent>& data);
			void OnKinematicbodyAdded(const ComponentAddedEvent<ECS::KinematicBody>& data);
			void OnRigidbodyAdded(const ComponentAddedEvent<ECS::RigidBodyComponent>& data);

		private:
			btBroadphaseInterface* broadphase = nullptr;
			btDefaultCollisionConfiguration* config = nullptr;
			btCollisionDispatcher* dispatcher = nullptr;
			btSequentialImpulseConstraintSolver* solver = nullptr;
			btDiscreteDynamicsWorld* world = nullptr;

			float simTimeInterval = 1 / 60.0f;
			float simTimer = 0.0f;
		};

	}
}

#endif