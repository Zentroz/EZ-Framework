#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include<filesystem>

#include"Engine/Render/Scene.h"
#include"Engine/Assets/AssetManager.h"
#include"Engine/ECS/Registry.h"
#include"Engine/Utils/TypeAlias.h"

namespace ENGINE {

	class Serializer {
	public:
		// Scene
		static void Serialize(Ref<Scene> scene);
		static bool Deserialize(const char* filepath, Ref<Scene> out_scene);

		// Project
		static void SerializeAssets(std::filesystem::path registryFilepath, MetaFileCache& metaCache);
		static void SerializeProject(ASSET::AssetManager& assetManager);
		static bool DeserializeProject(std::string filepath, ASSET::AssetManager& assetManager);
	private:
		Serializer() = default;
	};

}

#endif