# Vec3 & Ray math tests
add_executable(test_vec3
    test_vec3.cpp
)
target_link_libraries(test_vec3 PRIVATE
    raytracer_core
)
add_test(NAME TestVec3 COMMAND test_vec3)

# Sphere & Geometry tests
add_executable(test_geometry
    test_geometry.cpp
)
target_link_libraries(test_geometry PRIVATE
    raytracer_core
)
add_test(NAME TestGeometry COMMAND test_geometry)

# Textures & Perlin noise tests
add_executable(test_textures
    test_textures.cpp
)
target_link_libraries(test_textures PRIVATE
    raytracer_core
)
add_test(NAME TestTextures COMMAND test_textures)

# ImageBuffer RAII tests
add_executable(test_image_buffer
    test_image_buffer.cpp
)
target_link_libraries(test_image_buffer PRIVATE
    raytracer_core
)
add_test(NAME TestImageBuffer COMMAND test_image_buffer)

# CLI & SceneFactory tests
add_executable(test_cli_and_scenes
    test_cli_and_scenes.cpp
)
target_link_libraries(test_cli_and_scenes PRIVATE
    raytracer_core
)
add_test(NAME TestCliAndScenes COMMAND test_cli_and_scenes)
