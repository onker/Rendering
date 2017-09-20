// 02562 Rendering Framework
// Written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <optix_world.h>
#include "HitInfo.h"
#include "Plane.h"

using namespace optix;

bool Plane::intersect(const Ray& r, HitInfo& hit, unsigned int prim_idx) const
{
	float t_prime = -(dot(r.origin, onb.m_normal) + d) / (dot(r.direction, onb.m_normal));

	if (t_prime < r.tmin || t_prime > r.tmax) {
		return false;
	}
	else
	{
		hit.has_hit = true;
		hit.dist = t_prime;
		hit.position = r.origin + t_prime * r.direction;
		hit.geometric_normal = onb.m_normal;
		hit.shading_normal = onb.m_normal;
		hit.material = &material;
		return true;
	}
	return false;
}

void Plane::transform(const Matrix4x4& m)
{
  onb = Onb(normalize(make_float3(m*make_float4(onb.m_normal, 0.0f))));
  position = make_float3(m*make_float4(position, 1.0f));
  d = -dot(position, onb.m_normal);
}

Aabb Plane::compute_bbox() const
{
  return Aabb(make_float3(-1e37f), make_float3(1e37f));
}

void Plane::get_uv(const float3& hit_pos, float& u, float& v) const 
{ 
  // Do an inverse mapping from hit position to texture coordinates.
  //
  // Input:  hit_pos    (position where a ray intersected the plane)
  //
  // Output: u          (texture coordinate in the tangent direction of the plane)
  //         v          (texture coordinate in the binormal direction of the plane)
  //
  // Relevant data fields that are available (see Plane.h and OptiX math library reference)
  // position           (origin of the plane)
  // onb                (orthonormal basis of the plane: normal [n], tangent [b1], binormal [b2])
  // tex_scale          (constant for scaling the texture coordinates)

  u = 0.0f;
  v = 0.0f;
}