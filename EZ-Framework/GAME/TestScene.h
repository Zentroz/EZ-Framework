#pragma once

#include"Engine/Scene.h"

class TestScene : public Scene {
public:
	TestScene() : Scene("Test Scene") {};

	void Setup() override {
		Entity cube = registry->CreateEntity();
		registry->AddComponent(cube, new TransformComponent(float3::zero(), float3::one(), quaternion::Identity()));
		registry->AddComponent(cube, new MeshComponent("Assets/Mesh/cube.obj"));
		registry->AddComponent(cube, new MaterialComponent("Assets/Shaders/default.hlsl"));

		Entity cube2 = registry->CreateEntity();
		registry->AddComponent(cube2, new TransformComponent(float3(-3, 0, 0), float3::one(), quaternion::Identity()));
		registry->AddComponent(cube2, new MeshComponent("Assets/Mesh/cube.obj"));
		registry->AddComponent(cube2, new MaterialComponent("Assets/Shaders/default.hlsl"));

		Entity plane = registry->CreateEntity();
		registry->AddComponent(plane, new TransformComponent(float3(0, -1.1f, 0), float3(10, 1, 10), quaternion::Identity()));
		registry->AddComponent(plane, new MeshComponent("Assets/Mesh/plane.obj"));
		registry->AddComponent(plane, new MaterialComponent("Assets/Shaders/default.hlsl"));
	}
};