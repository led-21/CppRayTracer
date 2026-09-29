#ifndef RAYTRACER_RENDERER_RENDERER_HPP
#define RAYTRACER_RENDERER_RENDERER_HPP

#include "raytracer/core/cli.hpp"
#include "raytracer/core/constants.hpp"
#include "raytracer/core/random.hpp"
#include "raytracer/geometry/hittable.hpp"
#include "raytracer/materials/material.hpp"
#include "raytracer/math/color.hpp"
#include "raytracer/renderer/image_buffer.hpp"
#include "raytracer/scene/scene.hpp"

#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace raytracer {

class Renderer {
public:
    static color ray_color(const ray& r, const hittable& world, int depth, bool use_sky) {
        hit_record rec;

        // If exceeded ray bounce limit, no more light gathered
        if (depth <= 0) {
            return color(0.0, 0.0, 0.0);
        }

        if (world.hit(r, 0.001, infinity, rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat_ptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, world, depth - 1, use_sky);
            }
            return color(0.0, 0.0, 0.0);
        }

        if (!use_sky) {
            return color(0.0, 0.0, 0.0);
        }

        // Sky gradient background
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }

    static bool render(const Scene& scene, const RenderOptions& options) {
        const int width = options.image_width;
        const int height = options.image_height > 0
            ? options.image_height
            : static_cast<int>(width / (16.0 / 9.0));

        const int samples = options.samples_per_pixel;
        const int max_depth = options.max_depth;

        int num_threads = options.num_threads > 0
            ? options.num_threads
            : static_cast<int>(std::thread::hardware_concurrency());
        if (num_threads <= 0) {
            num_threads = 1;
        }

        std::cout << "========================================\n"
                  << " CppRayTracer - Multithreaded Renderer\n"
                  << "========================================\n"
                  << " Scene:          " << options.scene_name << "\n"
                  << " Resolution:     " << width << "x" << height << "\n"
                  << " Samples/pixel:  " << samples << "\n"
                  << " Max depth:      " << max_depth << "\n"
                  << " Threads:        " << num_threads << "\n"
                  << " Output:         " << options.output_path << "\n"
                  << "========================================\n" << std::endl;

        ImageBuffer image(width, height);
        const auto start_time = std::chrono::high_resolution_clock::now();

        std::atomic<int> next_scanline{0};
        std::atomic<int> completed_scanlines{0};

        auto render_worker = [&]() {
            while (true) {
                int j = next_scanline.fetch_add(1);
                if (j >= height) {
                    break;
                }

                for (int i = 0; i < width; ++i) {
                    color pixel_color(0.0, 0.0, 0.0);

                    for (int s = 0; s < samples; ++s) {
                        auto u = (i + random_double()) / (width - 1);
                        auto v = ((height - 1 - j) + random_double()) / (height - 1);
                        ray r = scene.cam.get_ray(u, v);
                        pixel_color += ray_color(r, scene.world, max_depth, scene.use_sky_gradient);
                    }

                    image.set_pixel(i, j, pixel_color, samples);
                }

                completed_scanlines.fetch_add(1);
            }
        };

        // Spawn worker threads
        std::vector<std::thread> workers;
        workers.reserve(static_cast<size_t>(num_threads));
        for (int t = 0; t < num_threads; ++t) {
            workers.emplace_back(render_worker);
        }

        // Live progress monitoring
        while (completed_scanlines.load() < height) {
            int done = completed_scanlines.load();
            int percent = (100 * done) / height;
            std::cerr << "\rRendering progress: " << std::setw(3) << percent << "% ("
                      << done << "/" << height << " scanlines) " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        std::cerr << "\rRendering progress: 100% (" << height << "/" << height << " scanlines) \n";

        const auto end_time = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> elapsed = end_time - start_time;

        const double total_primary_rays = static_cast<double>(width) * height * samples;
        const double mrays_per_second = (total_primary_rays / elapsed.count()) / 1'000'000.0;

        std::cout << "\nPerformance Metrics:\n"
                  << "  Elapsed Time:       " << std::fixed << std::setprecision(2) << elapsed.count() << " seconds\n"
                  << "  Primary Rays Cast:  " << std::fixed << std::setprecision(0) << total_primary_rays << "\n"
                  << "  Ray Throughput:     " << std::fixed << std::setprecision(2) << mrays_per_second << " MRays/s\n"
                  << std::endl;

        // Save output based on file extension
        bool saved = false;
        if (options.output_path.ends_with(".ppm") || options.output_path.ends_with(".PPM")) {
            saved = image.save_ppm(options.output_path);
        } else {
            saved = image.save_png(options.output_path);
        }

        if (saved) {
            std::cout << "[SUCCESS] Image saved to " << options.output_path << "\n";
        } else {
            std::cerr << "[ERROR] Failed to save image to " << options.output_path << "\n";
        }

        return saved;
    }
};

} // namespace raytracer

#endif // RAYTRACER_RENDERER_RENDERER_HPP
