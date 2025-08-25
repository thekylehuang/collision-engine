cmake_minimum_required(VERSION 3.20)
project(CollisionEngine VERSION 0.1.0 LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(glfw3)
find_package(glad)

add_executable(collision_engine src/main.cpp src/shader.cpp src/buffer_utils.cpp src/circle.cpp)

target_include_directories(collision_engine PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(collision_engine PRIVATE fmt::fmt glfw glad::glad)