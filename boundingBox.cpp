#include "boundingBox.h"

bool AABB::isInside(const Vector& p) 
{
	return
		(p.x > min.x && p.x < max.x) &&
		(p.y > min.y && p.y < max.y) &&
		(p.z > min.z && p.z < max.z);
}

bool AABB::intercepts(const Ray& ray, float& t)
{
	return true;
}