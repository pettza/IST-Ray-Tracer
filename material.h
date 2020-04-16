#pragma once

#include "color.h"

class Material
{
public:
	Material() :
		m_diffColor(Color(0.2f, 0.2f, 0.2f)),
		m_Diff(0.2f),
		m_specColor(Color(1.0f, 1.0f, 1.0f)),
		m_Spec(0.8f),
		m_Shine(20),
		m_Refl(1.0f),
		m_T(0.0f),
		m_RIndex(1.0f)
	{};

	Material(const Color& c, float Kd, const Color& cs, float Ks, float Shine, float T, float ior) :
		m_diffColor(c),
		m_Diff(Kd),
		m_specColor(cs),
		m_Spec(Ks),
		m_Shine(Shine),
		m_Refl(Ks),
		m_T(T),
		m_RIndex(ior)
	{};

	// Getters and Setters
	void SetDiffColor(const Color& color) { m_diffColor = color; }
	
	Color GetDiffColor() const { return m_diffColor; }
	
	void SetSpecColor(const Color& color) { m_specColor = color; }
	
	Color GetSpecColor() const { return m_specColor; }
	
	void SetDiffuse(float diff) { m_Diff = diff; }
	
	float GetDiffuse() const { return m_Diff; }
	
	void SetSpecular(float spec) { m_Spec = spec; }
	
	float GetSpecular() const { return m_Spec; }
	
	void SetShine(float shine) { m_Shine = shine; }
	
	float GetShine() const { return m_Shine; }
	
	void SetReflection(float refl) { m_Refl = refl; }
	
	float GetReflection() const { return m_Refl; }
	
	void SetTransmittance(float t) { m_T = t; }

	float GetTransmittance() const { return m_T; }
	
	void SetRefrIndex(float ior) { m_RIndex = ior; }
	
	float GetRefrIndex() const { return m_RIndex; }

private:
	// Member variables
	Color m_diffColor, m_specColor;
	float m_Refl, m_T;
	float m_Diff, m_Shine, m_Spec;
	float m_RIndex;
};