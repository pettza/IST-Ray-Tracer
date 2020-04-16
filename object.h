#pragma once

#include "shapes.h"
#include "material.h"

class Object
{
public:
	Object(Material* material = nullptr, Shape* shape = nullptr) :
		m_Material(material), m_Shape(shape)
	{}

	// Getter and Setter
	Material* GetMaterial() { return m_Material; }
	void SetMaterial(Material* material) { m_Material = material; }

	bool intercepts(const Ray& ray, HitInfo& hitInfo) const
	{
		hitInfo.incident = ray.direction;

		bool isHit = m_Shape->intercepts(ray, hitInfo);

		hitInfo.normal = Vector::FaceTowards(hitInfo.normal, -hitInfo.incident);

		if (!isHit) return false;

		hitInfo.material = m_Material;

		return true;
	}

	AABB GetBoundingBox() const { return m_Shape->GetBoundingBox(); }

private:
	Material* m_Material;
	Shape* m_Shape;
};