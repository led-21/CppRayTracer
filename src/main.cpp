#include "raytracer/core/cli.hpp"
#include "raytracer/renderer/renderer.hpp"
#include "raytracer/scene/scene_factory.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    using namespace raytracer;

    const auto options = CommandLineParser::parse(argc, argv);

    if (options.show_help) {
        CommandLineParser::print_usage(argv[0]);
        return 0;
    }

    const double aspect_ratio = (options.image_height > 0)
        ? static_cast<double>(options.image_width) / options.image_height
        : (16.0 / 9.0);

    const auto scene = SceneFactory::create_scene(options.scene_name, aspect_ratio);

    const bool success = Renderer::render(scene, options);

    return success ? 0 : 1;
}
