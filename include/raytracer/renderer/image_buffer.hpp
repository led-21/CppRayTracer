#ifndef RAYTRACER_RENDERER_IMAGE_BUFFER_HPP
#define RAYTRACER_RENDERER_IMAGE_BUFFER_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/math/color.hpp"

#include "stb_image_write.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace raytracer {

class ImageBuffer {
public:
    ImageBuffer(int width, int height)
        : width_(width), height_(height), data_(static_cast<size_t>(width * height * 3), 0) {}

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    const uint8_t* data() const noexcept { return data_.data(); }
    uint8_t* data() noexcept { return data_.data(); }

    void set_pixel(int x, int y, const color& pixel_color, int samples_per_pixel) {
        if (x < 0 || x >= width_ || y < 0 || y >= height_) {
            return;
        }

        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        // Handle NaN
        if (r != r) r = 0.0;
        if (g != g) g = 0.0;
        if (b != b) b = 0.0;

        // Scale by samples and apply gamma-2 correction
        const auto scale = 1.0 / samples_per_pixel;
        r = linear_to_gamma(scale * r);
        g = linear_to_gamma(scale * g);
        b = linear_to_gamma(scale * b);

        const size_t index = static_cast<size_t>(3 * (y * width_ + x));
        data_[index + 0] = static_cast<uint8_t>(256 * clamp(r, 0.0, 0.999));
        data_[index + 1] = static_cast<uint8_t>(256 * clamp(g, 0.0, 0.999));
        data_[index + 2] = static_cast<uint8_t>(256 * clamp(b, 0.0, 0.999));
    }

    bool save_png(const std::string& filename) const {
        int result = stbi_write_png(
            filename.c_str(),
            width_,
            height_,
            3,
            data_.data(),
            width_ * 3
        );
        return result != 0;
    }

    bool save_ppm(std::ostream& out) const {
        out << "P3\n" << width_ << ' ' << height_ << "\n255\n";
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                const size_t index = static_cast<size_t>(3 * (y * width_ + x));
                out << static_cast<int>(data_[index + 0]) << ' '
                    << static_cast<int>(data_[index + 1]) << ' '
                    << static_cast<int>(data_[index + 2]) << '\n';
            }
        }
        return out.good();
    }

    bool save_ppm(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out.is_open()) return false;
        return save_ppm(out);
    }

private:
    int width_{0};
    int height_{0};
    std::vector<uint8_t> data_;
};

} // namespace raytracer

#endif // RAYTRACER_RENDERER_IMAGE_BUFFER_HPP
