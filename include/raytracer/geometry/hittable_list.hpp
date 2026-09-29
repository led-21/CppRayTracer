#ifndef RAYTRACER_GEOMETRY_HITTABLE_LIST_HPP
#define RAYTRACER_GEOMETRY_HITTABLE_LIST_HPP

#include "raytracer/geometry/hittable.hpp"

#include <memory>
#include <vector>

namespace raytracer {

class hittable_list : public hittable {
public:
    hittable_list() = default;
    explicit hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(std::move(object)); }

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    std::vector<std::shared_ptr<hittable>> objects;
};

inline bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

} // namespace raytracer

#endif // RAYTRACER_GEOMETRY_HITTABLE_LIST_HPP