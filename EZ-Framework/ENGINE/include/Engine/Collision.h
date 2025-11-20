#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H

#include"Core/Math.h"

struct BoxCollider {
	float3 position;
	float3 halfExtent;
};

class Collision {
public:
	static bool AABBToAABB(BoxCollider a, BoxCollider b) {
		float3 aMax = a.position + a.halfExtent;
		float3 bMax = b.position + b.halfExtent;
		float3 aMin = a.position - a.halfExtent;
		float3 bMin = b.position - b.halfExtent;

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