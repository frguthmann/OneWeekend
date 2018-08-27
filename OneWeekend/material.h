#pragma once

#include "hitable.h"
#include "utility.h"

class material {
public:
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
	inline vec3 reflect(const vec3 &vec, const vec3 &normal) const { return vec - 2.0 * dot(vec, normal) * normal; }
	bool refract(const vec3 &v, const vec3& n, float ni_over_nt, vec3 & refracted) const {
		vec3 uv = unit_vector(v);
		float dt = dot(uv, n);
		float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1.0 - dt*dt);
		if (discriminant > 0) {
			refracted = ni_over_nt * (uv - n*dt) - n * sqrt(discriminant);
			return true;
		}
		else {
			return false;
		}
	}
	float schlick(float cosine, float ref_idx) const {
		float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0);
	}
};