#ifndef RAYTRACER_TEXTURES_TEXTURE_HPP
#define RAYTRACER_TEXTURES_TEXTURE_HPP

#include "raytracer/core/constants.hpp"
#include "raytracer/math/color.hpp"
#include "raytracer/textures/perlin.hpp"

#include "stb_image.h"

#include <iostream>
#include <memory>
#include <string>

namespace raytracer {

class texture {
public:
    virtual ~texture() = default;
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() : color_value_(0, 0, 0) {}
    solid_color(const color& c) : color_value_(c) {}
    solid_color(double red, double green, double blue)
        : color_value_(red, green, blue) {}

    virtual color value(double u, double v, const point3& p) const override {
        (void)u; (void)v; (void)p;
        return color_value_;
    }

private:
    color color_value_;
};

class checker_texture : public texture {
public:
    checker_texture() = default;

    checker_texture(std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
        : even_(std::move(even)), odd_(std::move(odd)) {}

    checker_texture(const color& c1, const color& c2)
        : even_(std::make_shared<solid_color>(c1)),
          odd_(std::make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = std::sin(10.0 * p.x()) * std::sin(10.0 * p.y()) * std::sin(10.0 * p.z());
        if (sines < 0.0) {
            return odd_->value(u, v, p);
        }
        return even_->value(u, v, p);
    }

private:
    std::shared_ptr<texture> even_;
    std::shared_ptr<texture> odd_;
};

class noise_texture : public texture {
public:
    noise_texture() : scale_(1.0) {}
    noise_texture(double sc) : scale_(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
        (void)u; (void)v;
        return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + std::sin(scale_ * p.z() + 10.0 * noise_.turb(p)));
    }

private:
    perlin noise_;
    double scale_;
};

class image_texture : public texture {
public:
    static constexpr int bytes_per_pixel = 3;

    image_texture()
        : data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

    explicit image_texture(const std::string& filename) {
        int components_per_pixel = bytes_per_pixel;
        int w = 0, h = 0;
        unsigned char* raw_data = stbi_load(filename.c_str(), &w, &h, &components_per_pixel, bytes_per_pixel);

        if (!raw_data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width_ = height_ = bytes_per_scanline_ = 0;
            return;
        }

        width_ = w;
        height_ = h;
        bytes_per_scanline_ = bytes_per_pixel * width_;
        data_ = std::shared_ptr<unsigned char>(raw_data, [](unsigned char* p) {
            if (p) stbi_image_free(p);
        });
    }

    virtual color value(double u, double v, const point3& p) const override {
        (void)p;
        // Solid cyan as debugging aid if image not loaded
        if (!data_) {
            return color(0.0, 1.0, 1.0);
        }

        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

        auto i = static_cast<int>(u * width_);
        auto j = static_cast<int>(v * height_);

        if (i >= width_) i = width_ - 1;
        if (j >= height_) j = height_ - 1;

        constexpr auto color_scale = 1.0 / 255.0;
        auto pixel = data_.get() + j * bytes_per_scanline_ + i * bytes_per_pixel;

        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    std::shared_ptr<unsigned char> data_;
    int width_{0};
    int height_{0};
    int bytes_per_scanline_{0};
};

} // namespace raytracer

#endif // RAYTRACER_TEXTURES_TEXTURE_HPP