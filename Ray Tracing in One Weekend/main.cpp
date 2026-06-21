#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "iostream"

color ray_color(const ray& r) {
	return color(0, 0, 0);
}


int main()
{
	// Image

	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	// Calculate image height. It has to be at least 1
	int image_height = int(image_width / aspect_ratio);
	if (image_height < 1) {
		image_height = 1;
	}

	// Camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	// Viewport vectors, which essentially store the viewport's dimensions
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// Pixel delta vectors
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// Location of top left corner
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v/2;

	// Position the top left pixel half a pixel delta from the edges
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {

			// Calculate pixel position. i represents x and j represents y
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center; // The direction in which the ray is pointed
			ray r(camera_center, ray_direction); // Create the ray

			color pixel_color = ray_color(r); // pixel_color stores the color of whatever the ray hits
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.                   \n";
}
