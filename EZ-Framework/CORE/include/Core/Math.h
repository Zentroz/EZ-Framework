#ifndef CORE_MATH_CLASS_H
#define CORE_MATH_CLASS_H

#include <cmath>

// # ------------------------------- Vectors -------------------------------------------- #

struct float2 {
	float x, y;

	float2 operator+(const float2& other) const {
		return { x + other.x, y + other.y };
	}
	float2 operator-(const float2& other) const {
		return { x - other.x, y - other.y };
	}
	float2& operator+=(const float2& other) {
		this->x += other.x;
		this->y += other.y;

		return *this;
	}
	float2 operator-=(const float2& other) {
		this->x -= other.x;
		this->y -= other.y;

		return *this;
	}
	float2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}
	float2 operator/(float scalar) const {
		return { x / scalar, y / scalar };
	}

	float length() {
		float distance = this->x * this->x + this->y * this->y;

		return sqrt(distance);
	}

	float2 normalize() {
		float length = this->length();

		if (length == 0) return float2(0, 0);

		return float2(this->x, this->y) / length;
	}

public:

	static float2 zero() {
		return float2(0, 0);
	}

	static float2 one() {
		return float2(1, 1);
	}

	static float2 up() {
		return float2(0, 1);
	}

	static float2 right() {
		return float2(1, 0);
	}
};

struct float3 {
	float x, y, z;

	float3() {
		x = 0;
		y = 0;
		z = 0;
	}

	float3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float3(const float2& f2) {
		this->x = f2.x;
		this->y = f2.y;
		this->z = 0;
	}

	float3 operator+(const float3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	float3 operator-(const float3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	float3& operator+=(const float3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;

		return *this;
	}
	float3 operator-=(const float3& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;

		return *this;
	}
	float3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}
	float3 operator/(float scalar) const {
		return { x / scalar, y / scalar, z / scalar };
	}

	double length() {
		float distance = this->x * this->x + this->y * this->y + this->z * this->z;

		return sqrt(distance);
	}

	float3 normalize() {
		float length = this->length();

		if (length == 0) return float3(0, 0, 0);

		return float3(this->x, this->y, this->z) / length;
	}

public:

	static float3 zero() {
		return float3(0, 0, 0);
	}

	static float3 one() {
		return float3(1, 1, 1);
	}

	static float3 up() {
		return float3(0, 1, 0);
	}

	static float3 right() {
		return float3(1, 0, 0);
	}

	static float3 forward() {
		return float3(0, 0, 1);
	}
};

struct float4 {
	float x, y, z, w;

	float4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	float4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	float4(const float3& f3) {
		this->x = f3.x;
		this->y = f3.y;
		this->z = f3.z;
		this->w = 0;
	}

	float4 operator+(const float4& other) const {
		return float4(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	float4 operator-(const float4& other) const {
		return float4(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	float4& operator+=(const float4& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;

		return *this;
	}
	float4 operator-=(const float4& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;

		return *this;
	}
	float4 operator*(float scalar) const {
		return float4(x * scalar, y * scalar, z * scalar, w * scalar);
	}
	float4 operator/(float scalar) const {
		return float4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	float length() {
		float distance = this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;

		return sqrt(distance);
	}

	float4 normalize() {
		float length = this->length();

		if (length == 0) return float4(0, 0, 0, 0);

		return float4(this->x, this->y, this->z, this->w) / length;
	}

public:

	static float4 zero() {
		return float4(0, 0, 0, 0);
	}

	static float4 one() {
		return float4(1, 1, 1, 1);
	}
};

#endif