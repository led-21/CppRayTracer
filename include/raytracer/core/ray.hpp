#ifndef RAYTRACER_CORE_RAY_HPP
#define RAYTRACER_CORE_RAY_HPP

#include "raytracer/math/vec3.hpp"

namespace raytracer {

class ray {
public:
    constexpr ray() noexcept = default;

    constexpr ray(const point3& origin, const vec3& direction, double time = 0.0) noexcept
        : orig(origin), dir(direction), tm(time) {}

    constexpr point3 origin() const noexcept { return orig; }
    constexpr vec3 direction() const noexcept { return dir; }
    constexpr double time() const noexcept { return tm; }

    constexpr point3 at(double t) const noexcept {
        return orig + t * dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm{0.0};
};

} // namespace raytracer

#endif // RAYTRACER_CORE_RAY_HPP