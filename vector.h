#pragma once

#include <iostream>

using namespace std;


struct Vector
{
	Vector(float x = 0.f, float y = 0.f, float z = 0.f)
		: x(x), y(y), z(z) {}
	
	Vector(const Vector& v) = default;

	// Operators
	Vector operator+(const Vector& v) const 
	{ return Vector(x + v.x, y + v.y, z + v.z); }

	Vector operator-(const Vector& v) const
	{ return Vector(x - v.x, y - v.y, z - v.z); }

	Vector operator*(float f) const
	{ return Vector(f * x, f * y, f * z); }
	
	friend inline
		Vector operator*(float f, const Vector& v)
	{ return v * f; }

	Vector operator/(float f) const
	{ return (*this) * (1 / f); }

	Vector operator-() const
	{ return Vector(-x, -y, -z); }

	float operator*(const Vector& v) const  //inner product
	{ return x * v.x + y * v.y + z * v.z; }

	Vector operator%(const Vector& v) const  //cross product
	{
		return Vector(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	Vector& operator=(const Vector& v) = default;

	Vector& operator+=(const Vector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector& operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}
	
	Vector&	operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return (*this);
	}
	
	Vector& operator/=(float f)
	{ return (*this) *= 1 / f; }

	float LengthSquared() const
	{ return x * x + y * y + z * z; }
	float Length() const
	{ return sqrtf(LengthSquared()); }
	
	Vector& Normalize() { return (*this) /= Length(); }

	static Vector FaceTowards(const Vector& a, const Vector& b)
	{ return (a * b > 0.f) ? a : -a; }

	// Member variables
	float x;
	float y;
	float z;

    friend inline
		istream& operator>>(istream& s, Vector& v)
	{ return s >> v.x >> v.y >> v.z; }
};
