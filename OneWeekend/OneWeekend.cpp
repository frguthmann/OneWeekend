// OneWeekend.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"


vec3 color (const ray& r, hitable_list * world, unsigned int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return 0;
		}
	}
	else {
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int store_pixel(vec3 &my_color, unsigned char * image, unsigned int cpt) {
	unsigned char ir(255.99*my_color[0]);
	unsigned char ig(255.99*my_color[1]);
	unsigned char ib(255.99*my_color[2]);
	image[cpt] = ir;
	image[cpt + 1] = ig;
	image[cpt + 2] = ib;
	return cpt + 3;
}

int main()
{
	const unsigned int nx = 200;
	const unsigned int ny = 100;
	const unsigned int ns = 1000;
	unsigned char * image = (unsigned char *) malloc(nx*ny*3*sizeof(unsigned char));

	unsigned int cpt = 0;
	std::cout << "Generating image of size: " << nx << " x " << ny << std::endl;
	vec3 lower_left_corner = vec3(-2.0, -1.0, -1.0);
	vec3 horizontal = vec3(4.0, 0.0, 0.0);
	vec3 vertical = vec3(0.0, 2.0, 0.0);
	vec3 origin = vec3(0.0, 0.0, 0.0);

	const unsigned int listSize = 4;
	hitable * list[listSize];
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));
	hitable_list * world = new hitable_list(list, listSize);
	camera cam;

	std::cout << std::fixed;
	std::cout << std::setprecision(2);

	for (int j = ny-1; j>=0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 my_color = vec3(0.0, 0.0, 0.0);
			for (int k = 0; k < ns; k++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray my_ray = cam.get_ray(u, v);
				my_color += color(my_ray, world, 0);
			}
			my_color /= ns;
			cpt = store_pixel(sqrt(my_color), image, cpt);
			//std::cout << "pix[" << i << ", " << j << "]: " << my_color << std::endl;
		}
		std::cout << "Progress: " << float(ny-j) * 100 / ny << "%" << std::endl;
	}

	std::stringstream name;
	name << "image_" << ns << "_spp_" << nx << "x" << ny << ".png";

	stbi_write_png(name.str().c_str(), nx, ny, 3, image, 3*nx);
	free(image);
	//getchar();
    return 0;
}

