#pragma once

#include<filesystem>
#include"Engine/Serialization/MetaFile.h"

namespace ENGINE {

	class AssetSerializer {
	public:
		static std::shared_ptr<MetaData> Deserialize(std::filesystem::path& path);
		template<typename T>
		static void Serialize(std::filesystem::path& path, T& meta);

		AssetSerializer() = default;

	private:
		static void SerializeTexture(std::filesystem::path& path, TextureMetaData& meta);
		static void SerializeShader(std::filesystem::path& path, ShaderMetaData& meta);
		static void SerializeMesh(std::filesystem::path& path, MeshMetaData& meta);
	};


	template<typename T>
	void AssetSerializer::Serialize(std::filesystem::path& path, T& meta) {
		if (std::filesystem::is_regular_file(path)) {
			std::string ext = path.extension().string();

			if (typeid(T) == typeid(TextureMetaData)) {
				SerializeTexture(path, (TextureMetaData&)meta);
			}
			else if (typeid(T) == typeid(MeshMetaData)) {
				SerializeMesh(path, (MeshMetaData&)meta);
			}
			else if (typeid(T) == typeid(ShaderMetaData)) {
				SerializeShader(path, (ShaderMetaData&)meta);
			}
		}
	}
}