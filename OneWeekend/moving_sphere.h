#pragma once

#include "hitable.h"

class moving_sphere : public hitable
{
public:
	moving_sphere() {}
	moving_sphere(const vec3& cen0, const vec3& cen1, float t0, float t1, float radius, material * mat) : 
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(radius), mat(mat) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;
	vec3 center(float time) const;

	vec3 center0, center1;
	float time0, time1;
	float radius;
	material * mat;
};

vec3 moving_sphere::center(float time) const {
	return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, float tmin, float tmax, hit_record &rec) const {
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius*radius;
	float delta = b*b - a*c;

	if (delta > 0.0) {
		float t = (-b - sqrt(delta)) / a;
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.material_ptr = mat;
			return true;
		}
		t = (-b + sqrt(delta)) / a;
		if (t < tmax && t > tmin) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.material_ptr = mat;
			return true;
		}
	}
	return false;
}