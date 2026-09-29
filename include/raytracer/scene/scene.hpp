#ifndef RAYTRACER_SCENE_SCENE_HPP
#define RAYTRACER_SCENE_SCENE_HPP

#include "raytracer/camera/camera.hpp"
#include "raytracer/geometry/hittable_list.hpp"
#include "raytracer/math/color.hpp"

namespace raytracer {

struct Scene {
    hittable_list world;
    camera cam;
    color background{0.7, 0.8, 1.0};
    bool use_sky_gradient{true};
};

} // namespace raytracer

#endif // RAYTRACER_SCENE_SCENE_HPP
