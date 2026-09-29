add_executable(test_vec3
    test_vec3.cpp
)

target_link_libraries(test_vec3 PRIVATE
    raytracer_core
)

add_test(NAME TestVec3 COMMAND test_vec3)
