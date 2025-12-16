#include"AssetSerializer.h"
#include<yaml-cpp/yaml.h>
#include<fstream>

namespace fs = std::filesystem;

namespace ENGINE {

	void AssetSerializer::SerializeTexture(std::filesystem::path& path, TextureMetaData& meta) {
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "euid" << YAML::Value << meta.euid.Get();
		out << YAML::Key << "type" << YAML::Value << "texture";

		out << YAML::Key << "ImportSettings" << YAML::BeginMap;

		out << YAML::Comment("Clamp = 0, Repeat = 1, Mirror = 2");
		out << YAML::Key << "wrapmode" << YAML::Value << (int)meta.wrapMode;
		out << YAML::Comment("Point = 0, Linear = 1");
		out << YAML::Key << "filtermode" << YAML::Value << (int)meta.filterMode;

		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream fout(path.string() + ".meta");
		fout << out.c_str();
	}
	void AssetSerializer::SerializeMesh(std::filesystem::path& path, MeshMetaData& meta) {
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "euid" << YAML::Value << meta.euid.Get();
		out << YAML::Key << "type" << YAML::Value << "mesh";

		out << YAML::Key << "ImportSettings" << YAML::BeginMap;

		out << YAML::Comment("0/1");
		out << YAML::Key << "read/write" << YAML::Value << meta.readWrite;
		out << YAML::Key << "scaleFactor" << YAML::Value << meta.scaleFactor;

		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream fout(path.string() + ".meta");
		fout << out.c_str();
	}
	void AssetSerializer::SerializeShader(std::filesystem::path& path, ShaderMetaData& meta) {
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "euid" << YAML::Value << meta.euid.Get();
		out << YAML::Key << "type" << YAML::Value << "shader";

		out << YAML::Key << "ImportSettings" << YAML::BeginMap;

		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream fout(path.string() + ".meta");
		fout << out.c_str();
	}

	bool DeserializeTexture(YAML::Node& root, TextureMetaData& meta) {
		auto importSettings = root["ImportSettings"];

		meta.wrapMode = (TextureWrapMode)importSettings["wrapmode"].as<int>();
		meta.filterMode = (TextureFilterMode)importSettings["filtermode"].as<int>();
		return true;
	}

	bool DeserializeMesh(YAML::Node& root, MeshMetaData& meta) {
		auto importSettings = root["ImportSettings"];

		meta.readWrite = importSettings["read/write"].as<bool>();
		meta.scaleFactor = importSettings["scaleFactor"].as<float>();
		return true;
	}

	bool DeserializeShader(YAML::Node& root, ShaderMetaData& meta) {
		return true;
	}

	std::shared_ptr<MetaData> AssetSerializer::Deserialize(fs::path& path) {
		YAML::Node node;
		try
		{
			node = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			return nullptr;
		}

		std::string type = node["type"].as<std::string>();
		std::string euid = node["euid"].as<std::string>();

		std::shared_ptr<MetaData> meta = nullptr;

		if (type == "texture") {
			std::shared_ptr<TextureMetaData> texMeta = std::make_shared<TextureMetaData>();
			if (!DeserializeTexture(node, *texMeta.get())) return nullptr;
			meta = texMeta;
		}
		else if (type == "mesh") {
			std::shared_ptr<MeshMetaData> meshMeta = std::make_shared<MeshMetaData>();
			if (!DeserializeMesh(node, *meshMeta.get())) return nullptr;
			meta = meshMeta;
		}
		else if (type == "shader") {
			std::shared_ptr<ShaderMetaData> shaderMeta = std::make_shared<ShaderMetaData>();
			if (!DeserializeShader(node, *shaderMeta.get())) return nullptr;
			meta = shaderMeta;
		}

		if (meta) {
			meta->euid = EUID(euid);
			meta->assetPath = path.string();
		}

		return meta;
	}

}