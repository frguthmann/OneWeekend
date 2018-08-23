#pragma once

#include "hitable.h"


#define drand48() (rand() / (RAND_MAX + 1.0))

class material {
public:
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
	vec3 random_in_unit_sphere() const;
	inline vec3 reflect(const vec3 &vec, const vec3 &normal) const { return vec - 2.0 * dot(vec, normal) * normal; }
};

vec3 material::random_in_unit_sphere() const{
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), drand48()) - 1.0;
	} while (p.squared_length() > 1);
	return p;
}