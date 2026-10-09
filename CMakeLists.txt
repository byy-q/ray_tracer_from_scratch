cmake_minimum_required(VERSION 3.16)

project(ray_tracer_from_scratch LANGUAGES CXX)

add_executable(ray_tracer
    main.cpp
)

target_compile_features(ray_tracer PRIVATE cxx_std_17)
target_include_directories(ray_tracer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
