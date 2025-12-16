#pragma once

#include<filesystem>
#include"Engine/Serialization/MetaFile.h"

namespace fs = std::filesystem;

namespace ENGINE {

    class AssetImporter {
    public:
        virtual ~AssetImporter() = default;
        virtual std::shared_ptr<MetaData> Import(const fs::path& path) = 0;
        bool CopyFileToFolder(const fs::path& filepath, const fs::path& destFolder);
    };

    class MeshImporter : public AssetImporter {
    public:
        virtual ~MeshImporter() = default;
        std::shared_ptr<MetaData> Import(const fs::path& path) override;
    };

    class TextureImporter : public AssetImporter {
    public:
        virtual ~TextureImporter() = default;
        std::shared_ptr<MetaData> Import(const fs::path& path) override;
    };

}