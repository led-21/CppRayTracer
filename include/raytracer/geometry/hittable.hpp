#ifndef RAYTRACER_GEOMETRY_HITTABLE_HPP
#define RAYTRACER_GEOMETRY_HITTABLE_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/core/ray.hpp"
#include "raytracer/math/vec3.hpp"

#include <memory>

namespace raytracer {

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t{0.0};
    double u{0.0};
    double v{0.0};
    bool front_face{false};

    inline void set_face_normal(const ray& r, const vec3& outward_normal) noexcept {
        front_face = dot(r.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

} // namespace raytracer

#endif // RAYTRACER_GEOMETRY_HITTABLE_HPP