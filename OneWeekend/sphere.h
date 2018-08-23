#pragma once

#include "hitable.h"

class sphere : public hitable 
{
public:
	sphere() {}
	sphere(const vec3& center, float radius, material * mat) : center(center), radius(radius), mat(mat) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;

	vec3 center;
	float radius;
	material * mat;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record &rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius*radius;
	float delta = b*b - a*c;

	if (delta > 0.0) {
		float t = (-b - sqrt(delta)) / a;
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = mat;
			return true;
		}
		t = (-b + sqrt(delta)) / a;
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = mat;
			return true;
		}
	}
	return false;
}