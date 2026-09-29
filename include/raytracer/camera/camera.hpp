#ifndef RAYTRACER_CAMERA_CAMERA_HPP
#define RAYTRACER_CAMERA_CAMERA_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/core/random.hpp"
#include "raytracer/core/ray.hpp"
#include "raytracer/math/vec3.hpp"

#include <cmath>

namespace raytracer {

class camera {
public:
    camera()
        : camera(point3(0, 0, -1), point3(0, 0, 0), vec3(0, 1, 0), 40.0, 1.0, 0.0, 10.0) {}

    camera(
        const point3& lookfrom,
        const point3& lookat,
        const vec3&   vup,
        double vfov, // vertical field-of-view in degrees
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double time0 = 0.0,
        double time1 = 0.0
    ) {
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2.0);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w_ = unit_vector(lookfrom - lookat);
        u_ = unit_vector(cross(vup, w_));
        v_ = cross(w_, u_);

        origin_ = lookfrom;
        horizontal_ = focus_dist * viewport_width * u_;
        vertical_ = focus_dist * viewport_height * v_;
        lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - focus_dist * w_;

        lens_radius_ = aperture / 2.0;
        time0_ = time0;
        time1_ = time1;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius_ * random_in_unit_disk();
        vec3 offset = u_ * rd.x() + v_ * rd.y();
        return ray(
            origin_ + offset,
            lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
            random_double(time0_, time1_)
        );
    }

private:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 u_, v_, w_;
    double lens_radius_{0.0};
    double time0_{0.0}, time1_{0.0}; // shutter open/close times
};

} // namespace raytracer

#endif // RAYTRACER_CAMERA_CAMERA_HPP