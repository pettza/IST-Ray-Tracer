#include "light.h"

#include "scene.h"
#include "maths.h"

Color Light::GetLighting(const Scene& scene, const HitInfo& hitInfo) const
{
	Vector L = position - hitInfo.hitP;
	float lightDist = L.Length();
	L /= lightDist;
	float lambert = L * hitInfo.normal;
	Color colorAcc(0.f, 0.f, 0.f);
	Ray rRay = hitInfo.GetReflectedRay();

	if (lambert <= 0.f)
	{
		if (hitInfo.material->GetTransmittance() == 0.f) return colorAcc;
		
		lambert = -lambert;
		rRay.origin = hitInfo.GetRefractedRay().origin;
	}
	
	Ray shadowRay(rRay.origin, L);

	HitInfo shadowInfo;
	bool occluded = scene.intercept(shadowRay, shadowInfo);

	if (occluded && shadowInfo.rayT < lightDist) return colorAcc;

	colorAcc += color * hitInfo.material->GetDiffColor() * hitInfo.material->GetDiffuse() * lambert;
	
	float shine = pow(max(0.f, L * rRay.direction), hitInfo.material->GetShine());
	colorAcc += color * hitInfo.material->GetSpecColor() * hitInfo.material->GetSpecular() * shine;

	return colorAcc;
}