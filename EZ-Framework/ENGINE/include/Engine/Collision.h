#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H

#include"Core/Math.h"

struct AABBCollider {
	float3 position;
	float3 halfExtents;

	AABBCollider(float3 position, float3 halfExtents) : position(position), halfExtents(halfExtents) {};
};

class Collision {
public:
	static bool AABBToAABB(AABBCollider a, AABBCollider b) {
		float3 aMax = a.position + a.halfExtents;
		float3 bMax = b.position + b.halfExtents;
		float3 aMin = a.position - a.halfExtents;
		float3 bMin = b.position - b.halfExtents;

		bool colliding = aMin.x <= bMax.x &&
			aMax.x >= bMin.x &&
			aMin.y <= bMax.y &&
			aMax.y >= bMin.y &&
			aMin.z <= bMax.z &&
			aMax.z >= bMin.z;

		return colliding;
	}
};

#endif