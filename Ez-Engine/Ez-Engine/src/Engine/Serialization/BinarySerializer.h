#pragma once

#include<string>
#include"Engine/Assets/RawAssets.h"

namespace ENGINE {
	class BinarySerializer {
	public:
		static void Write(const std::string file, char* str, std::streamsize size);
		static void Read(const std::string file, char* str, std::streamsize size);

		static void WriteMesh(const std::string file, RawMesh& mesh);
		static void ReadMesh(const std::string file, RawMesh& mesh);

		static void WriteTexture(const std::string file, RawTexture& texture);
		static void ReadTexture(const std::string file, RawTexture& texture);
	};
}