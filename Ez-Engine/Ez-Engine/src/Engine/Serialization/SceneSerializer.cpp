#include"Engine/Serialization/Serializer.h"
#include<yaml-cpp/yaml.h>
#include<fstream>

namespace YAML {
	template<>
	struct convert<float3>
	{
		static Node encode(const float3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, float3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<quaternion>
	{
		static Node encode(const quaternion& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, quaternion& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace ENGINE {

	YAML::Emitter& operator<<(YAML::Emitter& out, const float3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const quaternion& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void SerializeEntity(YAML::Emitter& out, EUID id, ECS::Registry* registry) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << id.Get();

		if (registry->Has<ECS::TransformComponent>(id)) {
			ECS::TransformComponent& t = registry->GetComponent<ECS::TransformComponent>(id);

			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Translation" << YAML::Value << t.position;
			out << YAML::Key << "Rotation" << YAML::Value << t.rotation;
			out << YAML::Key << "Scale" << YAML::Value << t.scale;

			out << YAML::EndMap;
		}

		if (registry->Has<ECS::MeshComponent>(id)) {
			ECS::MeshComponent& m = registry->GetComponent<ECS::MeshComponent>(id);

			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "MeshUUID" << YAML::Value << m.assetId.Get();

			out << YAML::EndMap;
		}

		if (registry->Has<ECS::MaterialComponent>(id)) {
			ECS::MaterialComponent& m = registry->GetComponent<ECS::MaterialComponent>(id);

			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "ShaderEUID" << YAML::Value << m.shaderAssetId.Get();
			out << YAML::Key << "TextureEUID" << YAML::Value << m.textureAssetId.Get();

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void Serializer::Serialize(Ref<Scene> scene) {
		if (!scene) return;
		Scene& s = scene->get();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << s.sceneName;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (EUID e : s.GetEntities()) {
			SerializeEntity(out, e, s.GetRegistry());
		}

		out << YAML::EndSeq; // Entity EndSeq
		out << YAML::EndMap;

		std::ofstream fout("Assets/Scene.yaml");
		fout << out.c_str();
	}

	bool Serializer::Deserialize(const char* filepath, Ref<Scene> out_scene) {
		if (!out_scene) return false;

		Scene& scene = out_scene->get();

		YAML::Node node;

		try
		{
			node = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			return false;
		} // Failed to load scene file

		if (!node["Scene"]) { return false; } // File doesn't contain scene map

		std::string sceneName = node["Scene"].as<std::string>();

		auto entities = node["Entities"];

		if (entities) {
			for (auto entity : entities) {
				//EUID loadedEntityId = EUID(entity["Entity"].as<std::string>());
				EUID loadedEntityId = scene.CreateEntity("");
				
				auto transform = entity["TransformComponent"];
				if (transform) {
					scene.GetRegistry()->AddComponent(loadedEntityId, ECS::TransformComponent(
						transform["Position"].as<float3>(),
						transform["Scale"].as<float3>(),
						transform["Rotation"].as<quaternion>()
					));
				}
			}
		}

		scene.sceneName = sceneName;

		return true;
	}

}