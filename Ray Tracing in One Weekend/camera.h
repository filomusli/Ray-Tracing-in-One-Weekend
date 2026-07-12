#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
public:
	/* Public Camera Parameters Here */
	double aspect_ratio = 1.0;
	int image_width = 100;

	void render(const hittable& world) {
		intialize();

		// Render
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {

				// Calculate pixel position. i represents x and j represents y
				auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				auto ray_direction = pixel_center - center; // The direction in which the ray is pointed
				ray r(center, ray_direction); // Create the ray

				color pixel_color = ray_color(r, world); // pixel_color stores the color of whatever the ray hits
				write_color(std::cout, pixel_color);
			}
		}

		std::clog << "\rDone.                   \n";

	}

private:
	/* Private Camera Variables Here */
	int		image_height;	// Rendered image height. Is calculated based on image width and aspect ratio
	point3	center;			// Camera center
	point3	pixel00_loc;	// Location in space of the top-left pixel in the viewport, half a pixel's length from edges
	vec3	pixel_delta_u;	// Pixel offset horizontally
	vec3	pixel_delta_v;	// Pixel offset vertically

	void intialize() {
		image_height = int(image_width / aspect_ratio); // Calculate image height
		image_height = (image_height < 1) ? 1 : image_height; // Make sure image height is at least 1 pixel

		center = point3(0, 0, 0); // Set camera position to 0, 0, 0 
		
		// Determine viewport dimensions
		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// Calculate horizontal and vertical vectors along viewport edges
		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		// Calculate horizontal and vertical distances between pixels
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate position in space of top-left pixel (0.5 of pixel width from edge)
		auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	}

	color ray_color(const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			return 0.5 * (rec.normal + color(1, 1, 1));
		}

		// The ray didn't hit the sphere
		// Calculate sky color
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif