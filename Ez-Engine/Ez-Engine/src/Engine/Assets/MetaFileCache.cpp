#include"Engine/Assets/MetaFileCache.h"
#include"Engine/Serialization/MetaSerializer.h"

namespace fs = std::filesystem;

void MetaFileCache::LoadAll(const fs::path& assetDir) {
	for (const auto& entry : fs::recursive_directory_iterator(assetDir)) {
		if (entry.is_regular_file()) {
			fs::path path = entry.path();
			Load(path);
		}
	}
}

MetaData* MetaFileCache::Load(const fs::path& filePath) {
	if (filePath.extension() != ".meta") return nullptr;

	auto meta = ENGINE::MetaSerializer::Read(filePath);
	
	if (meta) {
		meta->assetPath = (filePath.parent_path() / filePath.filename().stem()).string();
		m_MetaFiles[meta->euid] = meta;

		return meta.get();
	}

	return nullptr;
}


MetaData* MetaFileCache::Get(const EUID euid) {
	if (m_MetaFiles.contains(euid)) {
		return m_MetaFiles[euid].get();
	}

	return nullptr;
}

std::vector<MetaData*> MetaFileCache::GetOfType(const EUID euid, MetaAssetType type) {
	std::vector<MetaData*> metas;

	for (auto& [euid, meta] : m_MetaFiles) {
		if (meta->GetType() == type) metas.push_back(meta.get());
	}

	return metas;
}

void MetaFileCache::Add(std::shared_ptr<MetaData> meta) {
	m_MetaFiles[meta->euid] = meta;
}