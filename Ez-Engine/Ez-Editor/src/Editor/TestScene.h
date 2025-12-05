#pragma once

#include"Engine/Render/Scene.h"

using namespace MATH;

class TestScene : public ENGINE::Scene {
public:
	TestScene() : ENGINE::Scene("Test Scene") {};

	void Setup() override {
		/*
		for (short i = -1; i < 2; i++) {
			Entity e = registry->CreateEntity();

			registry->AddComponent(e, ENGINE::ECS::ColliderComponent(new ENGINE::ECS::BoxColliderData(float3::zero(), float3::one() / 2)));
			if (i == 0) {
				registry->AddComponent(e, ENGINE::ECS::TransformComponent(float3::up() / 2, float3::one(), quaternion::Identity()));
				registry->AddComponent(e, ENGINE::ECS::KinematicBody());
				registry->AddComponent(e, ENGINE::ECS::ScriptComponent("", "Player"));
			}
			else {
				registry->AddComponent(e, ENGINE::ECS::TransformComponent(float3(2 * i, 5, 0), float3::one(), quaternion::Identity()));
				registry->AddComponent(e, ENGINE::ECS::RigidBodyComponent());
			}

			registry->AddComponent(e, ENGINE::ECS::MeshComponent("Assets/Mesh/cube.obj"));
			registry->AddComponent(e, ENGINE::ECS::MaterialComponent("Assets/Shaders/default.hlsl", "Assets/Textures/White.png", float4(0.4f * abs(i), 0.5f, 0.4f * abs(i), 1)));
		}
		

		Entity e = registry->CreateEntity();
		registry->AddComponent(e, ENGINE::ECS::TransformComponent(float3(5, 5, 0), float3::one(), quaternion::Identity()));
		registry->AddComponent(e, ENGINE::ECS::MeshComponent("Assets/Mesh/cylinder.obj"));
		registry->AddComponent(e, ENGINE::ECS::MaterialComponent("Assets/Shaders/default.hlsl", "Assets/Textures/White.png", float4(0.8f, 0.05f, 0.15f, 1)));
		registry->AddComponent(e, ENGINE::ECS::ColliderComponent(new ENGINE::ECS::CylinderColliderData(float3::zero(), float3::one() / 2)));
		registry->AddComponent(e, ENGINE::ECS::RigidBodyComponent());

		Entity plane = registry->CreateEntity();
		registry->AddComponent(plane, ENGINE::ECS::TransformComponent(float3(0, 0, 0), float3(20, 1, 20), quaternion::Identity()));
		registry->AddComponent(plane, ENGINE::ECS::MeshComponent("Assets/Mesh/plane.obj"));
		registry->AddComponent(plane, ENGINE::ECS::MaterialComponent("Assets/Shaders/default.hlsl", "Assets/Textures/checker.png"));
		registry->AddComponent(plane, ENGINE::ECS::ColliderComponent(new ENGINE::ECS::StaticPlaneColliderData()));
		registry->AddComponent(plane, ENGINE::ECS::RigidBodyComponent(0)); */
	}
};