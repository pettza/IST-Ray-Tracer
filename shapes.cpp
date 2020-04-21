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
	normal = (p2 - p1) % (p0 - p1);
	normal.Normalize();
}

AABB Triangle::GetBoundingBox() const
{
	float min_x = min(points[0].x, min(points[1].x, points[2].x));
	float min_y = min(points[0].y, min(points[1].y, points[2].y));
	float min_z = min(points[0].z, min(points[1].z, points[2].z));

	float max_x = max(points[0].x, max(points[1].x, points[2].x));
	float max_y = max(points[0].y, max(points[1].y, points[2].y));
	float max_z = max(points[0].z, max(points[1].z, points[2].z));

	return AABB(Vector(min_x, min_y, min_z), Vector(max_x, max_y, max_z));
}

// Ray/Triangle intersection test using Tomas Moller-Ben Trumbore algorithm.
bool Triangle::intercepts(const Ray& ray, HitInfo& hitInfo) const
{
	float a = points[1].x - points[0].x;
	float b = points[2].x - points[0].x;
	float c = -ray.direction.x;
	float d = ray.origin.x - points[0].x;

	float e = points[1].y - points[0].y;
	float f = points[2].y - points[0].y;
	float g = -ray.direction.y;
	float h = ray.origin.y - points[0].y;

	float i = points[1].z - points[0].z;
	float j = points[2].z - points[0].z;
	float k = -ray.direction.z;
	float l = ray.origin.z - points[0].z;

	float det = a * (f * k - g * j) + b * (g * i - e * k) + c * (e * j - f * i);

	float beta = (d * (f * k - g * j) + b * (g * l - h * k) + c * (h * j - f * l)) / det;

	if (beta < 0.f || beta > 1.f) return false;

	float gamma = (a * (h * k - g * l) + d * (g * i - e * k) + c * (e * l - h * i)) / det;

	if (gamma < 0.f || gamma > 1.f) return false;

	float alpha = 1 - beta - gamma;

	if (alpha < 0.f || alpha > 1.f) return false;

	float t = (a * (f * l - h * j) + b * (h * i - e * l) + d * (e * j - f * i)) / det;

	if (t <= 0.f) return false;

	hitInfo.rayT = t;
	hitInfo.hitP = alpha * points[0] + beta * points[1] + gamma * points[2];
	hitInfo.normal = normal;

	return true;
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
	Vector min = center - radius * Vector(1.f, 1.f, 1.f);
	Vector max = center + radius * Vector(1.f, 1.f, 1.f);;

	return AABB(min, max);
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