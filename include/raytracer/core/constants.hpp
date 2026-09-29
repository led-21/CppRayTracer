#ifndef RAYTRACER_CORE_CONSTANTS_HPP
#define RAYTRACER_CORE_CONSTANTS_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace raytracer {

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = std::numbers::pi_v<double>;

constexpr double degrees_to_radians(double degrees) noexcept {
    return degrees * pi / 180.0;
}

template<typename T>
constexpr T clamp(T value, T min, T max) noexcept {
    return std::clamp(value, min, max);
}

} // namespace raytracer

#endif // RAYTRACER_CORE_CONSTANTS_HPP