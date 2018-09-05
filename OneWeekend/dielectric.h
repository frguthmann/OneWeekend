#pragma once

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {

		attenuation = vec3(1.0);
		vec3 outward_normal;
		float ni_over_nt;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = - dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		vec3 refracted;
		float reflect_pb;
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_pb = schlick(cosine, ref_idx);
		}
		else {
			reflect_pb = 1.0;
		}

		if (drand48() < reflect_pb) {
			vec3 reflected = reflect(r_in.direction(), rec.normal);
			scattered = ray(rec.p, reflected, r_in.time());
		}
		else {
			scattered = ray(rec.p, refracted, r_in.time());
		}

		return true;
	}

	float ref_idx;
};