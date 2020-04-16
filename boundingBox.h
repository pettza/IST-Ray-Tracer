#pragma once

#include "vector.h"
#include "ray.h"


class AABB
{
public:
	AABB() :
		min(Vector(-1.0f, -1.0f, -1.0f)),
		max(Vector(1.0f, 1.0f, 1.0f))
	{}

	AABB(const Vector& min, const Vector& max) : min(min), max(max) {}
	AABB(const AABB& bbox) = default;
	AABB& operator=(const AABB& rhs) = default;
	
	bool intercepts(const Ray& r, float& t);
	bool isInside(const Vector& p);

	// Member variables
	Vector min, max;
};