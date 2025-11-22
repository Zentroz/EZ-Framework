#include"Engine/PhysicsManager.h"

PhysicsManager::PhysicsManager() {
	broadphase = new btDbvtBroadphase();
	config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(config);
	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);

	world->setGravity(btVector3(0, -9.8f, 0));


	// Ground plane
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState* groundMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundCI(
		0.0f,
		groundMotion,
		groundShape
	);

	btRigidBody* groundBody = new btRigidBody(groundCI);
	world->addRigidBody(groundBody);

	EventBus::Subscribe<ComponentAddedEvent<ColliderComponent>>(
		[this](const ComponentAddedEvent<ColliderComponent>& data) {
			OnColliderAdded(data);
		}
	);
	EventBus::Subscribe<ComponentAddedEvent<RigidBodyComponent>>(
		[this](const ComponentAddedEvent<RigidBodyComponent>& data) {
			OnRigidbodyAdded(data);
		}
	);
}

void PhysicsManager::OnColliderAdded(const ComponentAddedEvent<ColliderComponent>& data) {
	if (data.registry->Has<RigidBodyComponent>(data.entity)) {
		RigidBodyComponent& rb = data.registry->GetComponent<RigidBodyComponent>(data.entity);

		rb.SetCollisionShape(data.component.data->GetBulletCollisionShape());
	}
}

void PhysicsManager::OnRigidbodyAdded(const ComponentAddedEvent<RigidBodyComponent>& data) {
	if (data.registry->Has<ColliderComponent>(data.entity)) {
		ColliderComponent& col = data.registry->GetComponent<ColliderComponent>(data.entity);

		data.component.SetCollisionShape(col.data->GetBulletCollisionShape());

		world->addRigidBody(data.component.bulletRb);
	}
}

void PhysicsManager::Update(Registry* registry) {
	float dt = GameTime::deltaTime;

	std::vector<Entity> physicsEntities = registry->view().Has<TransformComponent, RigidBodyComponent, ColliderComponent>().List();

	for (Entity e : physicsEntities) {
		TransformComponent t = registry->GetComponent<TransformComponent>(e);
		RigidBodyComponent r = registry->GetComponent<RigidBodyComponent>(e);
		ColliderComponent c = registry->GetComponent<ColliderComponent>(e);

		btTransform cubeTransform(btQuaternion(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w), btVector3(t.position.x, t.position.y, t.position.z));

		r.SetPosition(t.position);
	}

	world->stepSimulation(dt, 10, 1.0f / 240.0f);

	for (Entity e : physicsEntities) {
		TransformComponent& t = registry->GetComponent<TransformComponent>(e);
		RigidBodyComponent& r = registry->GetComponent<RigidBodyComponent>(e);
		ColliderComponent& c = registry->GetComponent<ColliderComponent>(e);

		btTransform cubeTransform;
		r.bulletRb->getMotionState()->getWorldTransform(cubeTransform);

		btVector3 btPos = cubeTransform.getOrigin();
		btQuaternion btQuat = cubeTransform.getRotation();

		t.position = float3(btPos.x(), btPos.y(), btPos.z());
		t.rotation = quaternion(btQuat.x(), btQuat.y(), btQuat.z(), btQuat.w());
	}
}