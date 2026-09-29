#include "raytracer/math/vec3.hpp"
#include "raytracer/core/ray.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << (msg) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::exit(1); \
        } \
    } while(0)

void test_vec3_operations() {
    vec3 v1(1.0, 2.0, 3.0);
    vec3 v2(4.0, 5.0, 6.0);

    // Addition
    vec3 add = v1 + v2;
    CHECK(std::fabs(add.x() - 5.0) < 1e-6, "vec3 addition x");
    CHECK(std::fabs(add.y() - 7.0) < 1e-6, "vec3 addition y");
    CHECK(std::fabs(add.z() - 9.0) < 1e-6, "vec3 addition z");

    // Subtraction
    vec3 sub = v2 - v1;
    CHECK(std::fabs(sub.x() - 3.0) < 1e-6, "vec3 subtraction x");
    CHECK(std::fabs(sub.y() - 3.0) < 1e-6, "vec3 subtraction y");
    CHECK(std::fabs(sub.z() - 3.0) < 1e-6, "vec3 subtraction z");

    // Dot product
    double d = dot(v1, v2);
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    CHECK(std::fabs(d - 32.0) < 1e-6, "vec3 dot product");

    // Cross product
    vec3 cp = cross(vec3(1, 0, 0), vec3(0, 1, 0));
    CHECK(std::fabs(cp.x() - 0.0) < 1e-6, "vec3 cross product x");
    CHECK(std::fabs(cp.y() - 0.0) < 1e-6, "vec3 cross product y");
    CHECK(std::fabs(cp.z() - 1.0) < 1e-6, "vec3 cross product z");

    // Length
    vec3 v3(3.0, 4.0, 0.0);
    CHECK(std::fabs(v3.length() - 5.0) < 1e-6, "vec3 length");

    // Unit vector
    vec3 uv = unit_vector(v3);
    CHECK(std::fabs(uv.length() - 1.0) < 1e-6, "vec3 unit vector");

    std::cout << "[PASS] test_vec3_operations\n";
}

void test_ray_operations() {
    point3 origin(1.0, 2.0, 3.0);
    vec3 direction(0.0, 1.0, 0.0);
    ray r(origin, direction);

    point3 at_zero = r.at(0.0);
    CHECK(std::fabs(at_zero.x() - 1.0) < 1e-6, "ray at t=0 x");
    CHECK(std::fabs(at_zero.y() - 2.0) < 1e-6, "ray at t=0 y");
    CHECK(std::fabs(at_zero.z() - 3.0) < 1e-6, "ray at t=0 z");

    point3 at_two = r.at(2.5);
    CHECK(std::fabs(at_two.x() - 1.0) < 1e-6, "ray at t=2.5 x");
    CHECK(std::fabs(at_two.y() - 4.5) < 1e-6, "ray at t=2.5 y");
    CHECK(std::fabs(at_two.z() - 3.0) < 1e-6, "ray at t=2.5 z");

    std::cout << "[PASS] test_ray_operations\n";
}

int main() {
    test_vec3_operations();
    test_ray_operations();
    std::cout << "All core math tests passed successfully!\n";
    return 0;
}
