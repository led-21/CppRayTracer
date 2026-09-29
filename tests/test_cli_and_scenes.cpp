#include "raytracer/core/cli.hpp"
#include "raytracer/scene/scene_factory.hpp"

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

void test_cli_parsing() {
    char arg0[] = "cpp-raytracer";
    char arg1[] = "--scene";
    char arg2[] = "materials";
    char arg3[] = "--width";
    char arg4[] = "640";
    char arg5[] = "--height";
    char arg6[] = "360";
    char arg7[] = "--samples";
    char arg8[] = "25";
    char arg9[] = "--depth";
    char arg10[] = "15";
    char arg11[] = "--threads";
    char arg12[] = "4";
    char arg13[] = "--output";
    char arg14[] = "custom_render.ppm";

    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14};
    int argc = 15;

    auto options = CommandLineParser::parse(argc, argv);

    CHECK(options.scene_name == "materials", "scene name should be materials");
    CHECK(options.image_width == 640, "width should be 640");
    CHECK(options.image_height == 360, "height should be 360");
    CHECK(options.samples_per_pixel == 25, "samples should be 25");
    CHECK(options.max_depth == 15, "depth should be 15");
    CHECK(options.num_threads == 4, "threads should be 4");
    CHECK(options.output_path == "custom_render.ppm", "output should be custom_render.ppm");
    CHECK(!options.show_help, "show_help should be false");

    // Test help flag
    char help_arg[] = "--help";
    char* help_argv[] = {arg0, help_arg};
    auto help_options = CommandLineParser::parse(2, help_argv);
    CHECK(help_options.show_help, "show_help should be true");

    std::cout << "[PASS] test_cli_parsing\n";
}

void test_scene_factory() {
    auto scenes = SceneFactory::available_scenes();
    CHECK(scenes.size() == 6, "there should be 6 registered scenes");

    for (const auto& scene_name : scenes) {
        auto scene = SceneFactory::create_scene(scene_name, 16.0 / 9.0);
        CHECK(!scene.world.objects.empty(), "scene world should not be empty");
    }

    std::cout << "[PASS] test_scene_factory\n";
}

int main() {
    test_cli_parsing();
    test_scene_factory();
    std::cout << "All CLI and SceneFactory tests passed successfully!\n";
    return 0;
}
