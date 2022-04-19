#pragma once

#include "Hittable.h"
#include "Vec3.h"

class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 center, double r) : center(center), radius(r) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

private:
	point3 center;
	double radius;
};

// See section 5.1 and 6.2 for the equation
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	// No solution for the quadratic equation
	if (discriminant < 0) return false;

	// Find the nearest root that lies in the acceptable range
	auto sqrt_d = sqrt(discriminant);
	auto root = (-half_b - sqrt_d) / a;
	if (root < t_min || root > t_max) {
		root = (-half_b + sqrt_d) / a;
		if (root < t_min || root > t_max) {
			return false;
		}
	}

	// Save the hit to hit_record
	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}

/*
// Quadratic formula: b^2 * t^2 + 2bt * (A - C) + (A - C) * (A - C) - r^2 = 0 (see notes)
 double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2.0 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4 * a * c;
	//return discriminant > 0;	// Check if the equation has solution, i.e, the ray is hitting the sphere

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}*/