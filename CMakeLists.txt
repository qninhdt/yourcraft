cmake_minimum_required(VERSION 3.0)
project(YourCraft)

# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(YourCraft
    src/main.cpp
    src/application.cpp
    src/camera.cpp
    src/resource.cpp
    src/graphic/display.cpp
    src/graphic/block_vertex.cpp
    src/graphic/skybox.cpp
    src/graphic/flora_vertex.cpp
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
    src/gui/gui.cpp
    src/gui/game_scene.cpp
    src/gui/pause_scene.cpp
    src/graphic/crosshair.cpp
    deps/glad/src/glad.c
    config.o
)

add_library(imgui SHARED
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_impl_glfw.cpp
    deps/imgui/imgui_impl_opengl3.cpp
    deps/imgui/imgui_tables.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui.cpp
)


target_link_libraries(imgui ${CMAKE_SOURCE_DIR}/build/glfw3.dll)
target_link_libraries(YourCraft imgui)
target_link_libraries(YourCraft ${CMAKE_SOURCE_DIR}/build/glfw3.dll)

include_directories(src)
include_directories(deps/glad/include)
include_directories(deps/GLFW)
include_directories(deps/glm)
include_directories(deps/imgui)
include_directories(deps)
