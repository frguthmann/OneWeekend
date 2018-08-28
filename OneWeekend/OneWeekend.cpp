// OneWeekend.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
	#include "stdafx.h"
	#include <windows.h>
	#define STBI_MSC_SECURE_CRT
#else
	#include <cfloat>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#define LOW 0
#define MED 1
#define HD 2
#define FULL_HD 3
#define FAST 0
#define MED 1
#define HIGH 2
#define FULL 3


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

void store_pixel(vec3 const &my_color, unsigned char * image, unsigned int cpt) {
	unsigned char ir(255.99*my_color[0]);
	unsigned char ig(255.99*my_color[1]);
	unsigned char ib(255.99*my_color[2]);
	image[cpt] = ir;
	image[cpt + 1] = ig;
	image[cpt + 2] = ib;
}

hitable_list * random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = drand48();
			vec3 center = vec3(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if ((center - vec3(4.0, 0.2, 0.0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1.0 + drand48()), 0.5 * (1.0 + drand48()), 0.5 * (1.0 + drand48())), 0.5 *drand48()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

#define RES FULL_HD
#define QUAL FULL

int main(int argc, char ** argv)
{

	#if RES == LOW
		const unsigned int nx = 200;
		const unsigned int ny = 100;
	#elif RES == MED
		const unsigned int nx = 720;
		const unsigned int ny = 480;
	#elif RES == HD
		const unsigned int nx = 1280;
		const unsigned int ny = 720;
	#elif RES == FULL_HD
		const unsigned int nx = 1920;
		const unsigned int ny = 1080;
	#endif

	#if (QUAL == FAST)
		const unsigned int ns = 10;
	#elif (QUAL == MED)
		const unsigned int ns = 100;
	#elif (QUAL == HIGH)
			const unsigned int ns = 500;
	#elif (QUAL == FULL)
		const unsigned int ns = 1000;
	#endif

    std::cout << omp_get_num_threads () << std::endl;
    int tsum=0;
    #pragma omp parallel
      tsum ++;
    std::cout << tsum << std::endl; 

	unsigned char * image = (unsigned char *) malloc(nx*ny*3*sizeof(unsigned char));

	unsigned int cpt = 0;
	std::cout << "Generating image of size: " << nx << " x " << ny << std::endl;
	vec3 lower_left_corner = vec3(-2.0, -1.0, -1.0);
	vec3 horizontal = vec3(4.0, 0.0, 0.0);
	vec3 vertical = vec3(0.0, 2.0, 0.0);
	vec3 origin = vec3(0.0, 0.0, 0.0);

	/*const unsigned int listSize = 5;
	hitable * list[listSize];
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.2));
	list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
	hitable_list * world = new hitable_list(list, listSize);*/


	hitable_list * world = random_scene();

	vec3 look_from(13, 2, 3);
	vec3 look_at(0, 0, 0);
	float focus_dist = 10.0;
	float aperture = 0.1;
	camera cam(look_from, look_at, vec3(0,1,0), 20, float(nx) / float(ny), aperture, focus_dist);

	std::cout << std::fixed;
	std::cout << std::setprecision(2);

	#pragma omp parallel for
	for (int j = ny-1; j>=0; j--) {
		#pragma omp parallel for
		for (int i = 0; i < nx; i++) {
			vec3 my_color = vec3(0.0, 0.0, 0.0);
			#pragma omp parallel for
			for (int k = 0; k < ns; k++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray my_ray = cam.get_ray(u, v);
				my_color += color(my_ray, world, 0);
			}
			my_color /= ns;
			unsigned int cpt = 3 * ((ny - j - 1)*nx + i);
			store_pixel(sqrt(my_color), image, cpt);
			//std::cout << "pix[" << i << ", " << j << "]: " << my_color << std::endl;
		}
		std::cout << "Progress: " << float(ny-j) * 100 / ny << "%" << std::endl;
	}

	std::stringstream name;
	name << "image_" << ns << "_spp_" << nx << "x" << ny << ".png";

	stbi_write_png(name.str().c_str(), nx, ny, 3, image, 3*nx);
	free(image);

	std::string imgPath = "%SystemRoot%\\System32\\rundll32.exe \"%ProgramFiles%\\Windows Photo Viewer\\PhotoViewer.dll\", ImageView_Fullscreen D:\\1_Francois\\Programmation\\RayTracing\\OneWeekend\\OneWeekend\\";
	imgPath.append(name.str().c_str());
	system(imgPath.c_str());

	//getchar();
    return 0;
}
