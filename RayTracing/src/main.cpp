#include <iostream>

#include "Rtweekend.h"
#include "Color.h"
#include "hittable_list.h"
#include "Sphere.h"




color ray_color(const ray& r, const hittable& world) {
	// Create color for objects hit by the ray
	hit_record record;
	if (world.hit(r, 0, infinity, record)) {
		return 0.5 * (record.normal + color(1, 1, 1));
	}

	// Linearly blends white and blue sky depending on the height of the y coordinates
	// blendedValue = (1 - t) * startValue + t * endValue
	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {

	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int> (image_width / aspect_ratio);

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));		// ball
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));	// ground

	// Camera
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";	// PPM format, P3 means ASCII

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < image_width; ++i) {
			double u = double(i) / (image_width - 1);
			double v = double(j) / (image_height - 1);

			// Create rays
			auto direction = lower_left_corner + u * horizontal + v * vertical - origin;
			ray r(origin, direction);

			// Paint color for hitted objects
			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\Image generated.\n";
}