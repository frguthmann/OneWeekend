#pragma once

#include "ray.h"

#define M_PI 3.14159265359

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
		float theta = vfov * M_PI / 180.0;
		float half_height = tan(theta/2);
		float half_width = half_height * aspect;
		origin = lookfrom;
		vec3 w = unit_vector(lookfrom - lookat);
		vec3 u = unit_vector(cross(vup, w));
		vec3 v = unit_vector(cross(w, u));
		lower_left_corner = origin - v*half_height - u*half_width - w;
		horizontal = 2.0*half_width*u;
		vertical = 2.0*half_height*v;
	}
	ray get_ray(float u, float v){return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};