#pragma once

#include "Vec3.h"
#include <iostream>

// Write the translated [0, 255] integer color value
void write_color(std::ostream& out, color pixel_color) {
	out << static_cast<int>(255.999 * pixel_color.x()) << ' '
		<< static_cast<int>(255.999 * pixel_color.y()) << ' '
		<< static_cast<int>(255.999 * pixel_color.z()) << '\n';
}