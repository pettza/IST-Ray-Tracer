#pragma once

#include "vector.h"
#include "material.h"

constexpr float shadow_epsilon = 1.e-4f;

struct Ray
{
	Ray(const Vector& o = Vector(), const Vector& dir = Vector()) : origin(o), direction(dir) {};

	Vector GetPoint(float t) const { return origin + t * direction; }

	void OffsetOrigin(const Vector& n)
	{ origin += shadow_epsilon * n; }

	Vector origin;
	Vector direction;
};


// Information about intersections
struct HitInfo
{
	HitInfo() : hitP(), incident(), normal(), rayT(INF), ior(), material(nullptr) {};
	HitInfo(Vector hitP, Vector incident, Vector normal, float rayT, float ior, Material* material) :
		hitP(hitP), incident(incident),
		normal(normal),
		rayT(rayT), ior(ior), material(material)
	{}
	HitInfo& operator=(const HitInfo&) = default;

	Ray GetReflectedRay() const
	{
		if (!cachedReflected)
		{
			cachedReflected = true;

			Vector i_n = normal * (incident * normal);
			
			reflected.origin = hitP;
			reflected.direction = incident - 2.f * i_n;

			reflected.OffsetOrigin(normal);
		}

		return reflected;
	}

	Ray GetRefractedRay() const
	{
		if (!cachedRefracted)
		{
			cachedRefracted = true;
			float ior2 = (ior == material->GetRefrIndex()) ? 1.f : material->GetRefrIndex();

			Vector i_n = normal * (incident * normal);
			Vector i_t = incident - i_n;
			Vector t_t = (ior / ior2) * i_t;

			float norm_magn_sq = 1 - t_t.LengthSquared();

			refracted.origin = hitP;
			refracted.OffsetOrigin(-normal);

			if (norm_magn_sq <= 0.f) refracted.direction = Vector(0.f, 0.f, 0.f);
			else refracted.direction = t_t - sqrtf(norm_magn_sq) * normal;
		}

		return refracted;
	}

	Vector hitP;
	Vector incident;
	mutable Ray reflected;
	mutable Ray refracted;
	Vector normal;
	float rayT;
	float ior;
	Material* material;

private:
	mutable bool cachedReflected = false;
	mutable bool cachedRefracted = false;
};