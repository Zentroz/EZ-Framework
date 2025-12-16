#include"Engine/Serialization/BinarySerializer.h"

#include<fstream>

namespace ENGINE {

	void BinarySerializer::Write(const std::string file, char* str, std::streamsize size) {		
		std::ofstream outfile(file, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outfile) return;

		outfile.write(str, size);
		outfile.close();
	}

	void BinarySerializer::Read(const std::string file, char* str, std::streamsize size) {
		std::ifstream infile(file, std::ios::binary);

		if (!infile) return;

		infile.read(str, size);
		infile.close();
	}

	void BinarySerializer::WriteMesh(const std::string file, RawMesh& mesh) {
		std::ofstream outfile(file, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outfile) return;

		uint64_t vertexCount = mesh.vertices.size();
		uint64_t indexCount = mesh.indexCount;

		outfile.write(reinterpret_cast<char*>(&vertexCount), sizeof(uint64_t));
		outfile.write(reinterpret_cast<char*>(&indexCount), sizeof(uint64_t));

		outfile.write(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(Vertex) * vertexCount);
		outfile.write(reinterpret_cast<char*>(mesh.indicies.data()), sizeof(uint32_t) * indexCount);

		outfile.close();
	}

	void BinarySerializer::ReadMesh(const std::string file, RawMesh& mesh) {
		std::ifstream infile(file, std::ios::binary);

		if (!infile) return;

		uint64_t vertexCount;
		uint64_t indexCount;

		infile.read(reinterpret_cast<char*>(&vertexCount), sizeof(uint64_t));
		infile.read(reinterpret_cast<char*>(&indexCount), sizeof(uint64_t));

		mesh.vertices.resize(vertexCount);
		mesh.indicies.resize(indexCount);
		mesh.indexCount = indexCount;

		infile.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(Vertex) * vertexCount);
		infile.read(reinterpret_cast<char*>(mesh.indicies.data()), sizeof(uint32_t) * indexCount);

		infile.close();
	}

	void BinarySerializer::WriteTexture(const std::string file, RawTexture& texture) {
		std::ofstream outfile(file, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!outfile) return;

		int width = texture.width;
		int height = texture.height;
		int channels = texture.channels;

		outfile.write(reinterpret_cast<char*>(&width), sizeof(int));
		outfile.write(reinterpret_cast<char*>(&height), sizeof(int));
		outfile.write(reinterpret_cast<char*>(&channels), sizeof(int));

		outfile.write(reinterpret_cast<char*>(texture.data), width * height * 4);

		outfile.close();
	}

	void BinarySerializer::ReadTexture(const std::string file, RawTexture& texture) {
		std::ifstream infile(file, std::ios::binary);

		if (!infile) return;

		infile.read(reinterpret_cast<char*>(&texture.width), sizeof(int));
		infile.read(reinterpret_cast<char*>(&texture.height), sizeof(int));
		infile.read(reinterpret_cast<char*>(&texture.channels), sizeof(int));

		infile.read(reinterpret_cast<char*>(texture.data), texture.width * texture.height * 4);

		infile.close();
	}

}