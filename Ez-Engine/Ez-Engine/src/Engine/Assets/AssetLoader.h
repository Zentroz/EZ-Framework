#pragma once

#include<string>
#include"Engine/Assets/RawAssets.h"

namespace ENGINE {
	class MeshLoader {
	public:
		static RawMesh Load(const std::string& filepath);
	private:
		MeshLoader() = default;
	};
	class TextureLoader {
	public:
		static RawTexture Load(const std::string& filepath);
	private:
		TextureLoader() = default;
	};
}