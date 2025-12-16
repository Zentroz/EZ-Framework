#include"Engine/Serialization/MetaSerializer.h"
#include<yaml-cpp/yaml.h>
#include<fstream>

namespace ENGINE {

	void WriteTexture(const std::filesystem::path& path, TextureMetaData& meta) {
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

		std::ofstream fout(path.string());
		fout << out.c_str();
	}
	void WriteMesh(const std::filesystem::path& path, MeshMetaData& meta) {
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

		std::ofstream fout(path.string());
		fout << out.c_str();
	}
	void WriteShader(const std::filesystem::path& filepath, ShaderMetaData& meta) {
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "euid" << YAML::Value << meta.euid.Get();
		out << YAML::Key << "type" << YAML::Value << "shader";

		out << YAML::Key << "ImportSettings" << YAML::BeginMap;

		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream fout(filepath.string());
		fout << out.c_str();
	}

	void MetaSerializer::Write(const fs::path& filepath, MetaData& meta) {
		if (meta.GetType() == MetaAssetType::MetaTexture) {
			WriteTexture(filepath, (TextureMetaData&)meta);
		}
		else if (meta.GetType() == MetaAssetType::MetaShader) {
			WriteShader(filepath, (ShaderMetaData&)meta);
		}
		else if (meta.GetType() == MetaAssetType::MetaMesh) {
			WriteMesh(filepath, (MeshMetaData&)meta);
		}
	}

	void ReadTexture(YAML::Node& root, TextureMetaData& meta) {
		auto importSettings = root["ImportSettings"];

		meta.wrapMode = (TextureWrapMode)importSettings["wrapmode"].as<int>();
		meta.filterMode = (TextureFilterMode)importSettings["filtermode"].as<int>();
	}

	void ReadShader(YAML::Node& root, ShaderMetaData& meta) {

	}

	void ReadMesh(YAML::Node& root, MeshMetaData& meta) {
		auto importSettings = root["ImportSettings"];

		meta.readWrite = importSettings["read/write"].as<bool>();
		meta.scaleFactor = importSettings["scaleFactor"].as<float>();
	}

	std::shared_ptr<MetaData> MetaSerializer::Read(const fs::path& path) {
		YAML::Node node;

		try
		{
			node = YAML::LoadFile(path.string());
		}
		catch (const YAML::ParserException e)
		{
			return nullptr;
		}

		std::shared_ptr<MetaData> meta = nullptr;

		YAML::Node euid = node["euid"];
		YAML::Node type = node["type"];

		if (!type || !euid) return nullptr;

		std::string typeStr = type.as<std::string>();

		if (typeStr == "texture") {
			std::shared_ptr<TextureMetaData> metaData = std::make_shared<TextureMetaData>();
			ReadTexture(node, *metaData);
			meta = metaData;
		}
		else if (typeStr == "shader") {
			std::shared_ptr<ShaderMetaData> metaData = std::make_shared<ShaderMetaData>();
			ReadShader(node, *metaData);
			meta = metaData;
		}
		else if (typeStr == "mesh") {
			std::shared_ptr<MeshMetaData> metaData = std::make_shared<MeshMetaData>();
			ReadMesh(node, *metaData);
			meta = metaData;
		}

		meta->euid = euid.as<std::string>();
		meta->assetPath = path.string();
		meta->metaPath = path.string();

		return meta;
	}
}