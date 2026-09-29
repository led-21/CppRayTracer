#ifndef RAYTRACER_MATH_COLOR_HPP
#define RAYTRACER_MATH_COLOR_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/math/vec3.hpp"

#include <cmath>
#include <iostream>

namespace raytracer {

inline double linear_to_gamma(double linear_component) noexcept {
    if (linear_component > 0.0) {
        return std::sqrt(linear_component);
    }
    return 0.0;
}

inline void write_color(std::ostream& out, const color& pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide color by the number of samples and apply gamma-2 correction.
    const auto scale = 1.0 / samples_per_pixel;
    r = linear_to_gamma(scale * r);
    g = linear_to_gamma(scale * g);
    b = linear_to_gamma(scale * b);

    // Write translated [0, 255] byte value.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

} // namespace raytracer

#endif // RAYTRACER_MATH_COLOR_HPP