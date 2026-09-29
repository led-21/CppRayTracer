#ifndef RAYTRACER_TEXTURES_PERLIN_HPP
#define RAYTRACER_TEXTURES_PERLIN_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/core/random.hpp"
#include "raytracer/math/vec3.hpp"

#include <array>
#include <cmath>
#include <numeric>

namespace raytracer {

class perlin {
public:
    static constexpr int point_count = 256;

    perlin() {
        for (int i = 0; i < point_count; ++i) {
            ranvec_[i] = unit_vector(vec3::random(-1.0, 1.0));
        }

        perm_x_ = perlin_generate_perm();
        perm_y_ = perlin_generate_perm();
        perm_z_ = perlin_generate_perm();
    }

    // Rule of Zero: default destructor, copy, and move operations work safely and correctly.
    ~perlin() = default;
    perlin(const perlin&) = default;
    perlin& operator=(const perlin&) = default;
    perlin(perlin&&) noexcept = default;
    perlin& operator=(perlin&&) noexcept = default;

    double noise(const point3& p) const noexcept {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = static_cast<int>(std::floor(p.x()));
        auto j = static_cast<int>(std::floor(p.y()));
        auto k = static_cast<int>(std::floor(p.z()));

        vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = ranvec_[
                        perm_x_[(i + di) & 255] ^
                        perm_y_[(j + dj) & 255] ^
                        perm_z_[(k + dk) & 255]
                    ];
                }
            }
        }

        return perlin_interp(c, u, v, w);
    }

    double turb(const point3& p, int depth = 7) const noexcept {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2.0;
        }

        return std::fabs(accum);
    }

private:
    std::array<vec3, point_count> ranvec_{};
    std::array<int, point_count> perm_x_{};
    std::array<int, point_count> perm_y_{};
    std::array<int, point_count> perm_z_{};

    static std::array<int, point_count> perlin_generate_perm() {
        std::array<int, point_count> p;
        std::iota(p.begin(), p.end(), 0);
        permute(p);
        return p;
    }

    static void permute(std::array<int, point_count>& p) {
        for (int i = point_count - 1; i > 0; --i) {
            int target = random_int(0, i);
            std::swap(p[i], p[target]);
        }
    }

    static double perlin_interp(const vec3 c[2][2][2], double u, double v, double w) noexcept {
        auto uu = u * u * (3.0 - 2.0 * u);
        auto vv = v * v * (3.0 - 2.0 * v);
        auto ww = w * w * (3.0 - 2.0 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1.0 - i) * (1.0 - uu)) *
                             (j * vv + (1.0 - j) * (1.0 - vv)) *
                             (k * ww + (1.0 - k) * (1.0 - ww)) *
                             dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }
};

} // namespace raytracer

#endif // RAYTRACER_TEXTURES_PERLIN_HPP