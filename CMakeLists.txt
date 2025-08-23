cmake_minimum_required(VERSION 3.20)
project(GravityEngine VERSION 0.1.0 LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(glfw3)
find_package(glad)

add_executable(gravity_engine src/main.cpp src/shader.cpp)

target_link_libraries(gravity_engine PRIVATE fmt::fmt glfw glad::glad)