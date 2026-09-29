#ifndef RAYTRACER_SCENE_SCENE_FACTORY_HPP
#define RAYTRACER_SCENE_SCENE_FACTORY_HPP

#include "raytracer/camera/camera.hpp"
#include "raytracer/core/random.hpp"
#include "raytracer/geometry/hittable_list.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/materials/material.hpp"
#include "raytracer/scene/scene.hpp"
#include "raytracer/textures/texture.hpp"

#include <memory>
#include <string>
#include <vector>

namespace raytracer {

class SceneFactory {
public:
    static std::vector<std::string> available_scenes() {
        return {
            "showcase",
            "basic-spheres",
            "materials",
            "dielectric-glass",
            "checker-spheres",
            "perlin-spheres"
        };
    }

    static Scene create_scene(const std::string& name, double aspect_ratio) {
        if (name == "basic-spheres") {
            return create_basic_spheres(aspect_ratio);
        } else if (name == "materials") {
            return create_materials_scene(aspect_ratio);
        } else if (name == "dielectric-glass") {
            return create_dielectric_glass_scene(aspect_ratio);
        } else if (name == "checker-spheres") {
            return create_checker_spheres(aspect_ratio);
        } else if (name == "perlin-spheres") {
            return create_perlin_spheres(aspect_ratio);
        } else {
            // Default to showcase scene
            return create_showcase(aspect_ratio);
        }
    }

private:
    static Scene create_basic_spheres(double aspect_ratio) {
        hittable_list world;

        auto ground_material = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
        auto center_material = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
        auto left_material   = std::make_shared<dielectric>(1.5);
        auto right_material  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

        world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, ground_material));
        world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, center_material));
        world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, left_material));
        world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, right_material));

        point3 lookfrom(3, 3, 2);
        point3 lookat(0, 0, -1);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.05;

        camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }

    static Scene create_materials_scene(double aspect_ratio) {
        hittable_list world;

        auto ground_material = std::make_shared<lambertian>(color(0.3, 0.3, 0.3));
        world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

        // Diffuse sphere
        world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0,
            std::make_shared<lambertian>(color(0.8, 0.2, 0.2))));

        // Pure specular mirror
        world.add(std::make_shared<sphere>(point3(-2, 1, 0), 1.0,
            std::make_shared<metal>(color(0.8, 0.85, 0.88), 0.0)));

        // Brushed metal (fuzz = 0.3)
        world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0,
            std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.3)));

        // Rough metal (fuzz = 0.8)
        world.add(std::make_shared<sphere>(point3(2, 1, 0), 1.0,
            std::make_shared<metal>(color(0.6, 0.6, 0.7), 0.8)));

        // Glass sphere
        world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0,
            std::make_shared<dielectric>(1.5)));

        point3 lookfrom(0, 3, 9);
        point3 lookat(0, 1, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.05;

        camera cam(lookfrom, lookat, vup, 28.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }

    static Scene create_dielectric_glass_scene(double aspect_ratio) {
        hittable_list world;

        auto checker = std::make_shared<checker_texture>(color(0.1, 0.1, 0.1), color(0.9, 0.9, 0.9));
        world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

        // Two colorful spheres behind the glass
        world.add(std::make_shared<sphere>(point3(-1.5, 0.8, -2.0), 0.8,
            std::make_shared<lambertian>(color(0.9, 0.1, 0.1))));
        world.add(std::make_shared<sphere>(point3(1.5, 0.8, -2.0), 0.8,
            std::make_shared<lambertian>(color(0.1, 0.3, 0.9))));

        // Hollow glass sphere in center (outer sphere r=1.0, inner sphere r=-0.8)
        auto glass_mat = std::make_shared<dielectric>(1.5);
        world.add(std::make_shared<sphere>(point3(0, 1.0, 0), 1.0, glass_mat));
        world.add(std::make_shared<sphere>(point3(0, 1.0, 0), -0.85, glass_mat));

        point3 lookfrom(0, 2, 4);
        point3 lookat(0, 1, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = (lookfrom - lookat).length();
        auto aperture = 0.0;

        camera cam(lookfrom, lookat, vup, 35.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }

    static Scene create_checker_spheres(double aspect_ratio) {
        hittable_list world;

        auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

        world.add(std::make_shared<sphere>(point3(0, -10, 0), 10.0, std::make_shared<lambertian>(checker)));
        world.add(std::make_shared<sphere>(point3(0,  10, 0), 10.0, std::make_shared<lambertian>(checker)));

        point3 lookfrom(13, 2, 3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;

        camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }

    static Scene create_perlin_spheres(double aspect_ratio) {
        hittable_list world;

        auto pertext = std::make_shared<noise_texture>(4.0);
        world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000.0, std::make_shared<lambertian>(pertext)));
        world.add(std::make_shared<sphere>(point3(0, 2, 0), 2.0, std::make_shared<lambertian>(pertext)));

        point3 lookfrom(13, 2, 3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.0;

        camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }

    static Scene create_showcase(double aspect_ratio) {
        hittable_list world;

        auto ground_material = std::make_shared<lambertian>(
            std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9))
        );
        world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

        for (int a = -5; a < 5; a++) {
            for (int b = -5; b < 5; b++) {
                auto choose_mat = random_double();
                point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

                if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                    std::shared_ptr<material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = std::make_shared<lambertian>(albedo);
                        world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    } else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = color::random(0.5, 1.0);
                        auto fuzz = random_double(0.0, 0.5);
                        sphere_material = std::make_shared<metal>(albedo, fuzz);
                        world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    } else {
                        // glass
                        sphere_material = std::make_shared<dielectric>(1.5);
                        world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }

        auto material1 = std::make_shared<dielectric>(1.5);
        world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

        point3 lookfrom(13, 2, 3);
        point3 lookat(0, 0, 0);
        vec3 vup(0, 1, 0);
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;

        camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);
        return Scene{std::move(world), cam, color(0.7, 0.8, 1.0), true};
    }
};

} // namespace raytracer

#endif // RAYTRACER_SCENE_SCENE_FACTORY_HPP
