#pragma once

#include<vector>
#include"Engine/Core/Math.h"

using namespace MATH;

namespace ENGINE {
	struct Vertex {
		float3 position;
		float3 normal;
		float2 uv;
	};

	struct RawMesh {
		std::vector<Vertex> vertices;
		std::vector<uint64_t> indicies;
		uint64_t indexCount = 0;
	};

	struct RawTexture {
		unsigned char* data;
		int width, height, channels;
	};
}