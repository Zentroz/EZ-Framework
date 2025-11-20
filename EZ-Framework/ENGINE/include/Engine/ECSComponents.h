#ifndef ECSCOMPONENT_CLASS_H
#define ECSCOMPONENT_CLASS_H

#include"Core/Math.h"

#include"ECSData.h"	
#include"sol/sol.hpp"

struct IComponent {
	Entity entity;

	static void RegisterWrapper(sol::state& m_lua) {} // Add and call this func for registering in Lua
};

struct TransformComponent : IComponent {
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

	static void RegisterLuaWrapper(sol::state& lua) {
		lua.new_usertype<TransformComponent>("Transform",
			sol::constructors<TransformComponent(), TransformComponent(float3, float3, quaternion)>(),
			sol::base_classes, sol::bases<IComponent>(),
			"position", &TransformComponent::position,
			"scale", &TransformComponent::scale,
			"rotation", &TransformComponent::rotation,
			"AddPosition", &TransformComponent::AddPosition
		);
	}
};

struct MeshComponent : IComponent {
	std::string path;

	MeshComponent(std::string path) : path(path) {}
};

struct MaterialComponent : IComponent {
	std::string materialName;
	std::string shaderPath;

	MaterialComponent(std::string shaderPath) : shaderPath(shaderPath) {}

	void SetFloat(std::string name, float value) {
		// To be implmented
	}
};

#endif