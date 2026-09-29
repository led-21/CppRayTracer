#include "raytracer/renderer/image_buffer.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace raytracer;

#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << (msg) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            std::exit(1); \
        } \
    } while(0)

void test_image_buffer_basic() {
    ImageBuffer buffer(2, 2);
    CHECK(buffer.width() == 2, "width should be 2");
    CHECK(buffer.height() == 2, "height should be 2");

    // Set pixel (0, 0) to red (1.0, 0.0, 0.0)
    buffer.set_pixel(0, 0, color(1.0, 0.0, 0.0), 1);

    // Set pixel (1, 1) to green (0.0, 1.0, 0.0)
    buffer.set_pixel(1, 1, color(0.0, 1.0, 0.0), 1);

    const uint8_t* raw = buffer.data();
    // Pixel (0, 0) should be ~255, 0, 0
    CHECK(raw[0] == 255, "pixel (0,0) red channel should be 255");
    CHECK(raw[1] == 0,   "pixel (0,0) green channel should be 0");
    CHECK(raw[2] == 0,   "pixel (0,0) blue channel should be 0");

    // Pixel (1, 1) index is 3 * (1 * 2 + 1) = 3 * 3 = 9
    CHECK(raw[9] == 0,   "pixel (1,1) red channel should be 0");
    CHECK(raw[10] == 255,"pixel (1,1) green channel should be 255");
    CHECK(raw[11] == 0,  "pixel (1,1) blue channel should be 0");

    // Test PPM stream generation
    std::ostringstream ss;
    bool ok = buffer.save_ppm(ss);
    CHECK(ok, "save_ppm should succeed");
    std::string ppm_str = ss.str();
    CHECK(ppm_str.find("P3\n2 2\n255\n") != std::string::npos, "PPM header should be present");

    std::cout << "[PASS] test_image_buffer_basic\n";
}

int main() {
    test_image_buffer_basic();
    std::cout << "All image buffer tests passed successfully!\n";
    return 0;
}
