#include "Color.h"
#include "Vec3.h"

#include <iostream>

int main() {

	// Image
	const int image_width = 256;
	const int image_height = 256;

	// Render
	// PPM format: P3 means colours are in ASCII
	// Then image_width columnes and image_height rows

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < image_width; ++i) {
			float r = float(i) / (image_width - 1);
			float g = float(j) / (image_height - 1);
			float b = 0.25;

			color pixel_color(r, g, b);
			write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
}