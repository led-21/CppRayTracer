#ifndef RAYTRACER_CORE_CLI_HPP
#define RAYTRACER_CORE_CLI_HPP

#include "raytracer/scene/scene_factory.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace raytracer {

struct RenderOptions {
    std::string scene_name{"showcase"};
    int image_width{800};
    int image_height{0}; // 0 = auto calculate based on 16:9 aspect ratio
    int samples_per_pixel{50};
    int max_depth{50};
    int num_threads{0};  // 0 = auto-detect hardware concurrency
    std::string output_path{"render.png"};
    bool show_help{false};
};

class CommandLineParser {
public:
    static void print_usage(const char* binary_name) {
        std::cout << "CppRayTracer - Modern C++ CPU Ray Tracer\n\n"
                  << "Usage:\n"
                  << "  " << binary_name << " [options]\n\n"
                  << "Options:\n"
                  << "  --scene <name>        Scene to render (default: showcase)\n"
                  << "  --width <int>         Image width in pixels (default: 800)\n"
                  << "  --height <int>        Image height in pixels (default: 0 = auto 16:9)\n"
                  << "  --samples <int>       Samples per pixel for antialiasing (default: 50)\n"
                  << "  --depth <int>         Maximum ray bounce depth (default: 50)\n"
                  << "  --threads <int>       Number of rendering threads (default: 0 = auto)\n"
                  << "  --output <file>       Output file path (.png or .ppm) (default: render.png)\n"
                  << "  -h, --help            Show this help message and list of scenes\n\n"
                  << "Available Scenes:\n";

        for (const auto& scene : SceneFactory::available_scenes()) {
            std::cout << "  - " << scene << "\n";
        }
        std::cout << std::endl;
    }

    static RenderOptions parse(int argc, char* argv[]) {
        RenderOptions options;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "-h" || arg == "--help") {
                options.show_help = true;
                return options;
            } else if (arg == "--scene" && i + 1 < argc) {
                options.scene_name = argv[++i];
            } else if (arg == "--width" && i + 1 < argc) {
                options.image_width = std::max(1, std::atoi(argv[++i]));
            } else if (arg == "--height" && i + 1 < argc) {
                options.image_height = std::max(0, std::atoi(argv[++i]));
            } else if (arg == "--samples" && i + 1 < argc) {
                options.samples_per_pixel = std::max(1, std::atoi(argv[++i]));
            } else if (arg == "--depth" && i + 1 < argc) {
                options.max_depth = std::max(1, std::atoi(argv[++i]));
            } else if (arg == "--threads" && i + 1 < argc) {
                options.num_threads = std::max(0, std::atoi(argv[++i]));
            } else if (arg == "--output" && i + 1 < argc) {
                options.output_path = argv[++i];
            } else {
                std::cerr << "Warning: Unknown or incomplete argument '" << arg << "'. Use --help for usage.\n";
            }
        }

        return options;
    }
};

} // namespace raytracer

#endif // RAYTRACER_CORE_CLI_HPP
