#pragma once

#include <iostream>

#include "maths.h"

using namespace std;

class Color
{
public:
    Color(float r = 0.f, float g = 0.f, float b = 0.f)
        : m_r(r), m_g(g), m_b(b)
    {}
    
    Color(const Color& c) = default;

    // Setters and Getters
    float r() const
	{ return m_r; }
    
    float r(float r)
	{ return m_r = r; }
    
    float g() const
    { return m_g; }
    
    float g(float g)
    { return m_g = g; }
    
    float b() const
    { return m_b; }
    
    float b(float b)
    { return m_b = b; }
    

    Color Clamp() const
    {
        return Color(
            clamp(m_r, 0.f, 1.f),
            clamp(m_g, 0.f, 1.f),
            clamp(m_b, 0.f, 1.f)
        );
    }
    
    // Operators
    Color operator+(const Color& c) 
    { return Color(m_r + c.m_r, m_g + c.m_g, m_b + c.m_b); }
    
    Color operator*(const Color& c) const
    { return Color(m_r * c.m_r, m_g * c.m_g, m_b * c.m_b); }
    
    Color operator*(float f) const 
    { return Color(m_r * f, m_g * f, m_b * f); }

    friend inline
        Color operator*(float f, const Color& c)
    { return c * f; }
    
    Color operator+=( Color c)
    {
        m_r += c.m_r;
        m_g += c.m_g;
        m_b += c.m_b;
        return *this;
    }
    
    Color operator*=(const Color& c)
	{
        m_r *= c.m_r;
        m_g *= c.m_g;
        m_b *= c.m_b;
        return *this;
    }

    
    Color operator*=(float f)
    {
        m_r *= f;
        m_g *= f;
        m_b *= f;
        return *this;
    }
    
   friend inline
       istream& operator>>(istream& s, Color& c)
	{ return s >> c.m_r >> c.m_g >> c.m_b; }

private:
    float m_r, m_g, m_b;
};