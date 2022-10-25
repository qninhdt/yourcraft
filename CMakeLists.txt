cmake_minimum_required(VERSION 3.0)
project(YourCraft)

add_executable(YourCraft
    src/main.cpp
    src/application.cpp
    src/camera.cpp
    src/resource.cpp
    src/graphic/display.cpp
    src/graphic/block_vertex.cpp
    src/graphic/skybox.cpp
    src/gl/mesh.cpp
    src/gl/common.cpp
    src/gl/shader.cpp
    src/gl/texture.cpp
    src/gl/shader.cpp
    src/gl/framebuffer.cpp
    src/graphic/block_outline.cpp
    src/world/chunk.cpp
    src/world/block.cpp
    src/world/world.cpp
    src/world/world_generator.cpp
    src/persistence.cpp
    src/player.cpp
    deps/glad/src/glad.c
)

# target_compile_options(YourCraft PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(YourCraft ${CMAKE_SOURCE_DIR}/build/glfw3.dll)
include_directories(src)
include_directories(deps/glad/include)
include_directories(deps/GLFW)
include_directories(deps/glm)
include_directories(deps)
