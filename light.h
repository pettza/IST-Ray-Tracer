#pragma once

#include "material.h"
#include "ray.h"

class Scene;

struct Light
{
public:
	Light(Vector& pos, Color& col) : position(pos), color(col)
	{};

	Color GetLighting(const Scene& scene, const HitInfo& hitInfo) const;

	Vector position;
	Color color;
};