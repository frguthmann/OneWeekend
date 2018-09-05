#pragma once

#include "vec3.h"

#define drand48() (rand() / (RAND_MAX + 1.0))
#ifndef M_PI
	#define M_PI 3.14159265359
#endif

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), drand48()) - 1.0;
	} while (p.squared_length() > 1);
	return p;
}

/*vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), 0.0) - vec3(1.0, 1.0, 0.0);
	} while (dot(p,p) >= 1.0);
	return p;
}*/

vec3 random_in_unit_disk() {
	float theta = drand48() * 2 * M_PI;
	float sqrt_r = sqrt(drand48());
	return vec3(sqrt_r * cos(theta), sqrt_r * sin(theta), 0.0);
}