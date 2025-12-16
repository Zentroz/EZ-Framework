#pragma once

#include<filesystem>
#include<unordered_map>

#include"Engine/Serialization/MetaFile.h"
#include"Engine/Utils/EUID.h"

class MetaFileCache {
public:
	MetaFileCache() = default;

	void LoadAll(const std::filesystem::path& assetDir);
	void Add(std::shared_ptr<MetaData> meta);

	MetaData* Get(const EUID euid);
	std::vector<MetaData*> GetOfType(const EUID euid, MetaAssetType type);
	MetaData* Load(const std::filesystem::path& filePath);

	const std::unordered_map<EUID, std::shared_ptr<MetaData>>& GetMetaMap() { return m_MetaFiles; }

private:
	std::unordered_map<EUID, std::shared_ptr<MetaData>> m_MetaFiles;
};