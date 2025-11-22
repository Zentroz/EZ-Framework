#pragma once

#include"Engine/Scene.h"

class TestScene : public Scene {
public:
	TestScene() : Scene("Test Scene") {};

	void Setup() override {

		for (short i = 0; i < 5; i++) {
			Entity e = registry->CreateEntity();
			registry->AddComponent(e, TransformComponent(float3(1.9f + 1.9f * i, 5 + 2 * i, 0), float3::one(), quaternion::Identity()));
			registry->AddComponent(e, MeshComponent("Assets/Mesh/cube.obj"));
			registry->AddComponent(e, MaterialComponent("Assets/Shaders/default.hlsl", "Assets/Textures/White.png", float4(0.1f * i, 0.1f * i, 0.1f * i, 1)));
			registry->AddComponent(e, ColliderComponent(new BoxColliderData(float3::zero(), float3::one())));
			registry->AddComponent(e, RigidBodyComponent());
		}

		Entity plane = registry->CreateEntity();
		registry->AddComponent(plane, TransformComponent(float3(0, 0, 0), float3(10, 1, 10), quaternion::Identity()));
		registry->AddComponent(plane, MeshComponent("Assets/Mesh/plane.obj"));
		registry->AddComponent(plane, MaterialComponent("Assets/Shaders/default.hlsl", "Assets/Textures/Cat.png"));
	}
};