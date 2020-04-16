#include "shapes.h"

#include "maths.h"

Plane::Plane(Vector& normal, float D)
	: normal(normal), D(D)
{}

Plane::Plane(Vector& p0, Vector& p1, Vector& p2) : D(0.f)
{
	//Calculate the normal plane: counter-clockwise vectorial product.
	normal = (p2 - p1) % (p0 - p1);

	if (normal.Length() == 0.0)
	{
		cerr << "DEGENERATED PLANE!\n";
	}
	else
	{
		normal.Normalize();

		//Calculate D
		D = normal * p0;
	}
}

AABB Plane::GetBoundingBox() const
{
	return AABB(Vector(-INF, -INF, -INF), Vector(INF, INF, INF));
}

// Ray/Plane intersection test.
bool Plane::intercepts(const Ray& ray, HitInfo& hitInfo) const
{
	float prod = normal * ray.direction;

	if (prod == .0) return false;

	float t_i = (D - ray.origin * normal) / prod;

	if (t_i <= 0.0) return false;

	hitInfo.hitP = ray.GetPoint(t_i);
	hitInfo.normal = normal;
	hitInfo.rayT = t_i;

	return true;
}

Triangle::Triangle(Vector& p0, Vector& p1, Vector& p2)
{
	points[0] = p0; points[1] = p1; points[2] = p2;

	/* Calculate the normal */
	normal = Vector(0, 0, 0);
	normal.Normalize();
}

AABB Triangle::GetBoundingBox() const
{
	return AABB();
}

// Ray/Triangle intersection test using Tomas Moller-Ben Trumbore algorithm.
bool Triangle::intercepts(const Ray& ray, HitInfo& hitInfo) const
{
	return false;
}

bool Sphere::intercepts(const Ray& ray, HitInfo& hitInfo) const
{
	float b, c;
	Vector oc = center - ray.origin;

	c = oc * oc - radius * radius;
	b = oc * ray.direction;

	if (c < .0) // ray inside sphere
	{
		hitInfo.rayT = b + sqrtf(b * b - c);
		hitInfo.hitP = ray.GetPoint(hitInfo.rayT);
		hitInfo.normal = (hitInfo.hitP - center).Normalize();

		return true;
	}
	else
	{
		float d = b * b - c;
		if (b > .0 && d > .0)
		{
			hitInfo.rayT = b - sqrtf(d);
			hitInfo.hitP = ray.GetPoint(hitInfo.rayT);
			hitInfo.normal = (hitInfo.hitP - center).Normalize();

			return true;
		}
	}

	return false;
}

AABB Sphere::GetBoundingBox() const
{
	Vector a_min;
	Vector a_max;
	return AABB(a_min, a_max);
}

aaBox::aaBox(Vector& minPoint, Vector& maxPoint) //Axis aligned Box: another geometric object
{
	min = minPoint;
	max = maxPoint;
}

AABB aaBox::GetBoundingBox() const
{
	return AABB(min, max);
}

bool aaBox::intercepts(const Ray& ray, HitInfo& hitInfo) const
{
	return false;
}