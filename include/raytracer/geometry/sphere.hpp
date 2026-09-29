#ifndef RAYTRACER_GEOMETRY_SPHERE_HPP
#define RAYTRACER_GEOMETRY_SPHERE_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/geometry/hittable.hpp"

#include <cmath>
#include <memory>

namespace raytracer {

class sphere : public hittable {
public:
    sphere() = default;

    sphere(const point3& cen, double r, std::shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(std::move(m)) {}

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

    static void get_sphere_uv(const point3& p, double& u, double& v) noexcept {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2.0 * pi);
        v = theta / pi;
    }

public:
    point3 center;
    double radius{0.0};
    std::shared_ptr<material> mat_ptr;
};

inline bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) return false;
    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

} // namespace raytracer

#endif // RAYTRACER_GEOMETRY_SPHERE_HPP