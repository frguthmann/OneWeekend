#pragma once

#include "hitable.h"

class sphere : public hitable 
{
public:
	sphere() {}
	sphere(const vec3& center, float radius) : center(center), radius(radius) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;

	vec3 center;
	float radius;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record &rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius*radius;
	float delta = b*b - 4*a*c;

	if (delta > 0.0) {
		float t = (-b - sqrt(delta)) / (2.0 * a);
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		t = (-b - sqrt(delta)) / (2.0 * a);
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}