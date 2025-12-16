#pragma once

#include<string>

#include"Engine/Utils/EUID.h"
#include"Engine/Assets/AssetDataTypes.h"

enum MetaAssetType { MetaTexture, MetaShader, MetaMesh, MetaUnknown };

struct MetaData {
public:
	EUID euid;
	std::string metaPath;
	std::string assetPath;

public:
	virtual ~MetaData() = default;
	virtual MetaAssetType GetType() { return MetaUnknown; }
};

struct TextureMetaData : public MetaData {
	TextureFilterMode filterMode;
	TextureWrapMode wrapMode;
	virtual MetaAssetType GetType() { return MetaTexture; }
};

struct MeshMetaData : public MetaData {
	bool readWrite;
	float scaleFactor;
	virtual MetaAssetType GetType() { return MetaMesh; }
};

struct ShaderMetaData : public MetaData {
	virtual MetaAssetType GetType() { return MetaShader; }
};