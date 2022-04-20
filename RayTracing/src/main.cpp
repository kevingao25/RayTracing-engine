#include <iostream>

#include "Rtweekend.h"
#include "Color.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

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
		/* // Diffuse the ray randomly to hit other objects
		point3 target = record.point + random_in_hemisphere(record.normal);
		return 0.5 * ray_color(ray(record.point, target - record.point), world, depth - 1);*/

		ray scattered;
		color attenuation;
		// Scatter the ray to shoot to different directions
		if (record.material_ptr->scatter(r, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return color(0, 0, 0);
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
	const int samples_per_pixel = 100;	// Hit 100 rays for each pixels (antialising)
	const int max_depth = 50;

	// World
	hittable_list world;

	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));		// ball
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));	// ground

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

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
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}


	std::cerr << "\nImage generated.\n";
	std::cerr << "Number of rays shooted: " << ray_count << std::endl;
}