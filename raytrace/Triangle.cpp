// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Triangle.h"

using namespace optix;

bool intersect_triangle(const Ray& ray, 
                        const float3& v0, 
                        const float3& v1, 
                        const float3& v2, 
                        float3& n,
                        float& t,
                        float& v,
                        float& w)
{
  // Implement ray-triangle intersection here (see Listing 1 in the lecture note).
  // Note that OptiX also has an implementation, so you can get away
  // with not implementing this function. However, I recommend that
  // you implement it for completeness.

  return false;
}


bool Triangle::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	float3 e_0 = v1 - v0;
	float3 e_1 = v0 - v2;
	float3 n = cross(e_0, e_1);
	float3 triangleVertexToOrigin = v0 - r.origin;

	float v = dot(cross(triangleVertexToOrigin, r.direction), e_1) / dot(r.direction, n);
	float w = dot(cross(triangleVertexToOrigin, r.direction), e_0) / dot(r.direction, n);
	float u = 1 - v - w;

	float t = dot(triangleVertexToOrigin, n) / dot(r.direction, n);

	if (v >= 0 && w >= 0 && v + w <= 1
		&& t >= r.tmin && t <= r.tmax)
	{
		hit.has_hit = true;
		hit.dist = t;
		hit.position = r.origin + t * r.direction;
		hit.geometric_normal = n;
		hit.shading_normal = n;
		hit.material = &material;

		return true;
	}
	else
	{
		return false;
	}
}

void Triangle::transform(const Matrix4x4& m) 
{ 
  v0 = make_float3(m*make_float4(v0, 1.0f)); 
  v1 = make_float3(m*make_float4(v1, 1.0f)); 
  v2 = make_float3(m*make_float4(v2, 1.0f)); 
}

Aabb Triangle::compute_bbox() const
{
  Aabb bbox;
  bbox.include(v0);
  bbox.include(v1);
  bbox.include(v2);
  return bbox;
}
