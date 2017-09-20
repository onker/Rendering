// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Sphere.h"

using namespace optix;

bool Sphere::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	float a = 1;
	float b_half = dot((r.origin - center), r.direction);
	float temp = dot((r.origin - center), (r.origin - center));
	float c = temp - pow(radius,2);
	float t_prime_1 = -b_half - sqrt(pow(b_half, 2) - c);
	float t_prime_2 = -b_half + sqrt(pow(b_half, 2) - c);
	float t = t_prime_1;
	if (pow(b_half, 2) - c < 0)
	{
		return false;
	}
	if (t < r.tmin || t > r.tmax) 
	{
		t = t_prime_2;
		if (t < r.tmin || t > r.tmax) 
		{
			return false;
		}
	}
	hit.has_hit = true;
	hit.dist = t;
	hit.position = r.origin + t_prime_1 * r.direction;
	hit.geometric_normal = hit.position - center;
	hit.shading_normal = hit.position - center;
	hit.material = &material;
	return true;
}

void Sphere::transform(const Matrix4x4& m)
{
  float3 radius_vec = make_float3(radius, 0.0f, 0.0f) + center;
  radius_vec = make_float3(m*make_float4(radius_vec, 1.0f));
  center = make_float3(m*make_float4(center, 1.0f)); 
  // The radius is scaled by the X scaling factor.
  // Not ideal, but the best we can do without elipsoids
  radius_vec -= center;
  radius = length(radius_vec);  
}

Aabb Sphere::compute_bbox() const
{
  Aabb bbox;
  bbox.include(center - make_float3(radius, 0.0f, 0.0f));
  bbox.include(center + make_float3(radius, 0.0f, 0.0f));
  bbox.include(center - make_float3(0.0f, radius, 0.0f));
  bbox.include(center + make_float3(0.0f, radius, 0.0f));
  bbox.include(center - make_float3(0.0f, 0.0f, radius));
  bbox.include(center + make_float3(0.0f, 0.0f, radius));
  return bbox;
}
