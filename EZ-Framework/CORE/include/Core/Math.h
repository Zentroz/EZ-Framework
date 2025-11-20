#ifndef CORE_MATH_CLASS_H
#define CORE_MATH_CLASS_H

#include <cmath>

const double PI = 3.14159265359;
const float Rad2Deg = 180 / PI;
const float Deg2Rad = PI / 180;

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

struct quaternion {
	float x, y, z, w;

	quaternion() = default;
	quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	static quaternion FromAxisAngle(float3 axis, float angleRad) {
		float half = angleRad * 0.5f;
		float s = sinf(half);
		return quaternion(
			axis.x * s,
			axis.y * s,
			axis.z * s,
			cosf(half)
		);
	}

	static quaternion Identity() { return quaternion(0, 0, 0, 1); }

	quaternion operator*(const quaternion& q) const {
		return quaternion(
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w,
			w * q.w - x * q.x - y * q.y - z * q.z
		);
	}

	void normalize() {
		float len = sqrtf(w * w + x * x + y * y + z * z);
		w /= len; x /= len; y /= len; z /= len;
	}

	float3 forward() const {
		return {
			2 * (x * z + y * w),
			2 * (y * z - x * w),
			1 - 2 * (x * x + y * y)
		};
	}

	float3 right() const {
		return {
			1 - 2 * (y * y + z * z),
			2 * (x * y + w * z),
			2 * (x * z - w * y)
		};
	}

	float3 up() const {
		return {
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x)
		};
	}
};

#endif