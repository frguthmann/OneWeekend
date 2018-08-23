#pragma once

#include "material.h"

class metal : public material {
public:
	metal(const vec3 &albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

	vec3 albedo;
	float fuzz;
};