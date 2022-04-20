#pragma once

#include "Ray.h"
#include "Rtweekend.h"

class material;

struct hit_record {
	point3 point;
	vec3 normal;	// Our setup is normal always point outward
	shared_ptr<material> material_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

