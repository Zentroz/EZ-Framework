#include"Engine/Serialization/Serializer.h"
#include"Engine/Serialization/MetaFile.h"

#include<yaml-cpp/yaml.h>
#include<fstream>

namespace fs = std::filesystem;

namespace ENGINE {

	void Serializer::SerializeProject(ASSET::AssetManager& assetManager) {
		
	}

	bool Serializer::DeserializeProject(std::string filepath, ASSET::AssetManager& assetManager) {
		return true;
	}

	void Serializer::SerializeAssets(fs::path registryFilepath, MetaFileCache& metaCache) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (const auto& [euid, meta] : metaCache.GetMetaMap()) {
			out << YAML::BeginMap;

			out << YAML::Key << "euid" << YAML::Value << meta->euid.Get();
			out << YAML::Key << "importPath" << YAML::Value << meta->assetPath;

			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(registryFilepath);
		fout << out.c_str();
	}
}