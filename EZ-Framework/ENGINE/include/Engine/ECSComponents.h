#ifndef ECSCOMPONENT_CLASS_H
#define ECSCOMPONENT_CLASS_H

#include"Core/Math.h"

#include"ECSData.h"	
#include"sol/sol.hpp"

struct IComponent {
	Entity entity;

	static void RegisterWrapper() {}
};

struct ScriptComponent : IComponent {
	std::string path;
	sol::environment luaEnv;

	ScriptComponent(std::string path) {
		this->path = path;
	}
};

struct TransformComponent : IComponent {
	float3 position;
	float3 scale;

	TransformComponent() {
		position = float3(0.0f, 0.0f, 0.0f);
		scale = float3(1.0f, 1.0f, 1.0f);
	}

	TransformComponent(float3 position, float3 scale) {
		this->position = position;
		this->scale = scale;
	}

	void AddPosition(float3 delta) {
		position = position + delta;
	}

	static void RegisterWrapper(sol::state* lua) {
		lua->new_usertype<TransformComponent>("Transform",
			sol::constructors<TransformComponent(), TransformComponent(float3, float3)>(),
			"position", &TransformComponent::position,
			"scale", &TransformComponent::scale,
			"AddPosition", &TransformComponent::AddPosition
		);
	}
};

struct MeshComponent : IComponent {
	std::string meshName;
};

struct MaterialComponent : IComponent {
	std::string materialName;

	void SetFloat(std::string name, float value) {
		// Placeholder for setting a float parameter in the material
	}
};

#endif