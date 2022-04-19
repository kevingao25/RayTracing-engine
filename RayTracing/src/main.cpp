#include <iostream>

#include "Rtweekend.h"
#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Camera.h"

// Debug
int ray_count = 0;

color ray_color(const ray& r, const hittable& world, int depth) {
	// Debug
	ray_count++;

	// Create color for objects hit by the ray
	hit_record record;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, record)) {	// use 0.001 to fix shadow acne problem
		// Diffuse the ray randomly to hit other objects
		point3 target = record.p + record.normal + random_unit_vector();
		return 0.5 * ray_color(ray(record.p, target - record.p), world, depth - 1);
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
	const int samples_per_pixel = 100;	// Hit 100 rays for each pixels
	const int max_depth = 50;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));		// ball
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));	// ground

	// Camera
	camera cam;

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";	// PPM format, P3 means ASCII

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < image_width; ++i) {

			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);

				// Debug
				ray_count++;
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}


	std::cerr << "\nImage generated.\n";
	std::cerr << "Number of rays shooted: " << ray_count << std::endl;
}