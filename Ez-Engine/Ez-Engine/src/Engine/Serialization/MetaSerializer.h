#pragma once

#include<filesystem>
#include"Engine/Serialization/MetaFile.h"

namespace fs = std::filesystem;

namespace ENGINE {
	class MetaSerializer {
	public:
		static void Write(const fs::path& filepath, MetaData& meta);
		static std::shared_ptr<MetaData> Read(const fs::path& path);
	private:
		MetaSerializer() = default;
	};

}