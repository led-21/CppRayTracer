#ifndef RAYTRACER_CORE_RANDOM_HPP
#define RAYTRACER_CORE_RANDOM_HPP

#include <random>

namespace raytracer {

inline double random_double() {
    thread_local std::mt19937_64 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    thread_local std::mt19937_64 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

} // namespace raytracer

#endif // RAYTRACER_CORE_RANDOM_HPP
