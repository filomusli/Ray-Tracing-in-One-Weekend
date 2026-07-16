#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
public:
	/* Public Camera Parameters Here */
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	void render(const hittable& world) {
		intialize();

		// Render
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

			for (int i = 0; i < image_width; i++) {

				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone.                   \n";

	}

private:
	/* Private Camera Variables Here */
	int		image_height;	// Rendered image height. Is calculated based on image width and aspect ratio
	double	pixel_samples_scale; // Scale factor for collection pixel samples
	point3	center;			// Camera center
	point3	pixel00_loc;	// Location in space of the top-left pixel in the viewport, half a pixel's length from edges
	vec3	pixel_delta_u;	// Pixel offset horizontally
	vec3	pixel_delta_v;	// Pixel offset vertically

	void intialize() {
		image_height = int(image_width / aspect_ratio); // Calculate image height
		image_height = (image_height < 1) ? 1 : image_height; // Make sure image height is at least 1 pixel

		pixel_samples_scale = 1.0 / samples_per_pixel;

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

	ray get_ray(int i, int j) const {
		// Create a ray from camera center pointing at randomly sampled point around pixel [i, j]

		auto offset = sample_square();
		auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0) {
			return color(0, 0, 0);
		}


		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			// vec3 direction = random_on_hemisphere(rec.normal);
			vec3 direction = rec.normal + random_unit_vector();
			return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
			
			// return 0.5 * (rec.normal + color(1, 1, 1));
		}

		// The ray didn't hit the sphere
		// Calculate sky color
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif