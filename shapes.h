#pragma once

#include "boundingBox.h"
#include "ray.h"

// Shape abstract class
class Shape
{
public:
	virtual bool intercepts(const Ray& ray, HitInfo& hitInfo) const = 0;
	virtual AABB GetBoundingBox() const = 0;
};

class Plane : public Shape
{
public:
	Plane(Vector& PNc, float Dc);
	Plane(Vector& P0, Vector& P1, Vector& P2);

	virtual bool intercepts(const Ray& ray, HitInfo& hitInfo) const;
	virtual AABB GetBoundingBox() const;

private:
	Vector	 normal;
	float 	 D;
};

class Triangle : public Shape
{
public:
	Triangle(Vector& p0, Vector& p1, Vector& p2);
	virtual bool intercepts(const Ray& ray, HitInfo& hitInfo) const;
	virtual AABB GetBoundingBox() const;

private:
	Vector points[3];
	Vector normal;
};


class Sphere : public Shape
{
public:
	Sphere(Vector& center, float radius) :
		center(center),
		radius(radius) {};

	virtual bool intercepts(const Ray& ray, HitInfo& hitInfo) const;
	virtual AABB GetBoundingBox() const;

private:
	Vector center;
	float radius;
};

class aaBox : public Shape   //Axis aligned box: another geometric object
{
public:
	aaBox(Vector& minPoint, Vector& maxPoint);
	virtual bool intercepts(const Ray& r, HitInfo& hitInfo) const;
	virtual AABB GetBoundingBox() const;

private:
	Vector min;
	Vector max;
};