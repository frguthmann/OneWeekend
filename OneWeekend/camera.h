#pragma once

#include "ray.h"
#include "utility.h"

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float fd, float t0 , float t1) {
		time0 = t0;
		time1 = t1;
		float theta = vfov * M_PI / 180.0;
		focus_dist = fd;
		float half_height = tan(theta/2) * focus_dist;
		float half_width = half_height * aspect;
		origin = lookfrom;
		vec3 w = unit_vector(lookfrom - lookat);
		vec3 u = unit_vector(cross(vup, w));
		vec3 v = unit_vector(cross(w, u));
		lower_left_corner = origin - v*half_height - u*half_width - w*focus_dist;
		horizontal = 2.0*half_width*u;
		vertical = 2.0*half_height*v;
		lens_radius = aperture / 2.0;
	}
	ray get_ray(float u, float v){
		vec3 random = lens_radius * random_in_unit_disk();
		vec3 offset = random.x() * u + random.y() * v;
		float time = time0 + drand48() * (time1 - time0);
		return ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - (origin + offset), time);
	}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	float lens_radius;
	float focus_dist;
	float time0, time1;
};