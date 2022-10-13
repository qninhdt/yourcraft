cmake_minimum_required(VERSION 3.0)
project(YourCraft)

add_executable(YourCraft
    src/main.cpp
    src/application.cpp
    src/camera.cpp
    src/resource.cpp
    src/graphic/display.cpp
    src/graphic/block_vertex.cpp
    src/gl/mesh.cpp
    src/gl/common.cpp
    src/gl/shader.cpp
    src/gl/texture.cpp
    src/gl/shader.cpp
    src/world/chunk.cpp
    src/world/block.cpp
    deps/glad/src/glad.c
)

target_link_libraries(YourCraft ${CMAKE_SOURCE_DIR}/build/glfw3.dll)
include_directories(src)
include_directories(deps/glad/include)
include_directories(deps/GLFW/include)
include_directories(deps/stb)
include_directories(deps/glm/include)
