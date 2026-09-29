#include "raytracer/textures/texture.hpp"
#include "raytracer/textures/perlin.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace raytracer;

#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << (msg) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::exit(1); \
        } \
    } while(0)

void test_checker_texture() {
    color c1(1.0, 1.0, 1.0);
    color c2(0.0, 0.0, 0.0);
    checker_texture checker(c1, c2);

    // Test value at point (0.05, 0.05, 0.05)
    color val1 = checker.value(0, 0, point3(0.05, 0.05, 0.05));
    // sin(10*0.05) = sin(0.5) > 0. All 3 positive -> product positive -> even (c1)
    CHECK(val1.x() == 1.0 && val1.y() == 1.0 && val1.z() == 1.0, "checker val1 should be c1");

    std::cout << "[PASS] test_checker_texture\n";
}

void test_perlin_rule_of_zero() {
    // Test that Perlin noise can be constructed, copied, and assigned without crash
    perlin p1;
    double n1 = p1.noise(point3(1.5, 2.5, 3.5));
    CHECK(n1 >= -1.0 && n1 <= 1.0, "perlin noise should be between -1 and 1");

    // Copy constructor test
    perlin p2 = p1;
    double n2 = p2.noise(point3(1.5, 2.5, 3.5));
    CHECK(n1 == n2, "copied perlin should produce identical noise values");

    // Copy assignment test
    perlin p3;
    p3 = p1;
    double n3 = p3.noise(point3(1.5, 2.5, 3.5));
    CHECK(n1 == n3, "assigned perlin should produce identical noise values");

    // Turbulence test
    double turb = p1.turb(point3(2.0, 3.0, 4.0));
    CHECK(turb >= 0.0, "turbulence should be non-negative");

    std::cout << "[PASS] test_perlin_rule_of_zero\n";
}

int main() {
    test_checker_texture();
    test_perlin_rule_of_zero();
    std::cout << "All texture & noise tests passed successfully!\n";
    return 0;
}
