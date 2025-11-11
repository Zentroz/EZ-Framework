#ifndef MATH_CLASS_H
#define MATH_CLASS_H

struct float3 {
	float x, y, z;

	float3 operator+(const float3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	float3 operator-(const float3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	float3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}
	float3 operator/(float scalar) const {
		return { x / scalar, y / scalar, z / scalar };
	}
};

struct float2 {
	float x, y;

	float2 operator+(const float2& other) const {
		return { x + other.x, y + other.y };
	}
	float2 operator-(const float2& other) const {
		return { x - other.x, y - other.y };
	}
	float2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}
	float2 operator/(float scalar) const {
		return { x / scalar, y / scalar };
	}
};

#endif