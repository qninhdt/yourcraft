#version 440 core

layout (location = 0) in vec3 coord;
layout (location = 1) in vec2 pass_uv;

uniform mat4 projection_view;
uniform mat4 model;

out vec2 uv;

void main() {
    gl_Position = projection_view * model * vec4(coord, 1.0);
    uv = pass_uv;
}  