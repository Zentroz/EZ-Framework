#include"Engine/Assets/AssetImporter.h"
#include"Engine/Engine.h"
#include"Engine/Serialization/MetaSerializer.h"
#include"Engine/Serialization/BinarySerializer.h"
#include"Engine/Assets/AssetLoader.h"

namespace ENGINE {

	bool AssetImporter::CopyFileToFolder(const fs::path& filepath, const fs::path& destFolder) {
		fs::path dest = destFolder / filepath.filename();

		if (!fs::is_directory(destFolder)) fs::create_directory(destFolder);

		try {
			fs::copy_file(filepath, dest, fs::copy_options::overwrite_existing);
		}
		catch (fs::filesystem_error e) {
			return false;
		}

		return true;
	}

	std::shared_ptr<MetaData> MeshImporter::Import(const fs::path& path) {
		RawMesh raw = MeshLoader::Load(path.string());

		if (raw.indexCount == 0) return nullptr;

		fs::path assetDir = Engine::Get().Project().GetAssetDirectory();
		fs::path libraryDir = Engine::Get().Project().GetLibraryDirectory();

		if (!CopyFileToFolder(path, assetDir)) return nullptr;

		fs::path copiedAssetFilepath = assetDir / path.filename();
		fs::path metaPath = copiedAssetFilepath.string() + ".meta";

		std::shared_ptr<MeshMetaData> meta = std::make_unique<MeshMetaData>();
		MetaSerializer::Write(metaPath, *meta);

		fs::path binFilepath = libraryDir / meta->euid.Get();

		BinarySerializer::WriteMesh(binFilepath.string(), raw);

		meta->assetPath = binFilepath.string();
		meta->metaPath = metaPath.string();

		return meta;
	}

	std::shared_ptr<MetaData> TextureImporter::Import(const fs::path& path) {
		RawTexture raw = TextureLoader::Load(path.string());

		fs::path assetDir = Engine::Get().Project().GetAssetDirectory();
		fs::path libraryDir = Engine::Get().Project().GetLibraryDirectory();

		if (!CopyFileToFolder(path, assetDir)) return nullptr;

		fs::path copiedAssetFilepath = assetDir / path.filename();
		fs::path metaPath = copiedAssetFilepath.string() + ".meta";

		std::shared_ptr<TextureMetaData> meta = std::make_unique<TextureMetaData>();
		MetaSerializer::Write(metaPath, *meta);

		fs::path binFilepath = libraryDir / meta->euid.Get();

		BinarySerializer::WriteTexture(binFilepath.string(), raw);

		meta->assetPath = binFilepath.string();
		meta->metaPath = metaPath.string();
	}
}