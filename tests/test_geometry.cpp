#include "raytracer/geometry/sphere.hpp"
#include "raytracer/materials/material.hpp"

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

void test_sphere_hit() {
    auto mat = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    sphere s(point3(0, 0, -5), 1.0, mat);

    // Direct hit through center
    ray r_hit(point3(0, 0, 0), vec3(0, 0, -1));
    hit_record rec;
    bool hit = s.hit(r_hit, 0.001, 100.0, rec);
    CHECK(hit, "ray should hit sphere");
    CHECK(std::fabs(rec.t - 4.0) < 1e-5, "hit distance should be 4.0");
    CHECK(std::fabs(rec.p.z() - (-4.0)) < 1e-5, "hit point z should be -4.0");
    CHECK(std::fabs(rec.normal.z() - 1.0) < 1e-5, "normal z should point towards ray origin");
    CHECK(rec.front_face, "hit should be on front face");

    // Ray missing sphere
    ray r_miss(point3(0, 2.5, 0), vec3(0, 0, -1));
    hit_record rec_miss;
    bool miss = s.hit(r_miss, 0.001, 100.0, rec_miss);
    CHECK(!miss, "ray should miss sphere");

    // Ray pointing away from sphere
    ray r_away(point3(0, 0, 0), vec3(0, 0, 1));
    hit_record rec_away;
    bool away = s.hit(r_away, 0.001, 100.0, rec_away);
    CHECK(!away, "ray pointing away should not hit sphere");

    std::cout << "[PASS] test_sphere_hit\n";
}

void test_sphere_uv() {
    double u = 0.0;
    double v = 0.0;

    // North pole (0, 1, 0): theta = acos(-1) = pi -> v = 1.0
    sphere::get_sphere_uv(point3(0, 1, 0), u, v);
    CHECK(std::fabs(v - 1.0) < 1e-5, "north pole v should be 1.0");

    // South pole (0, -1, 0): theta = acos(1) = 0 -> v = 0.0
    sphere::get_sphere_uv(point3(0, -1, 0), u, v);
    CHECK(std::fabs(v - 0.0) < 1e-5, "south pole v should be 0.0");

    // Equator (1, 0, 0): theta = pi/2 -> v = 0.5, phi = atan2(0, 1) + pi = pi -> u = 0.5
    sphere::get_sphere_uv(point3(1, 0, 0), u, v);
    CHECK(std::fabs(u - 0.5) < 1e-5, "equator (1,0,0) u should be 0.5");
    CHECK(std::fabs(v - 0.5) < 1e-5, "equator (1,0,0) v should be 0.5");

    std::cout << "[PASS] test_sphere_uv\n";
}

int main() {
    test_sphere_hit();
    test_sphere_uv();
    std::cout << "All geometry tests passed successfully!\n";
    return 0;
}
