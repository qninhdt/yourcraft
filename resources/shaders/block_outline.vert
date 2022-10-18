#version 440 core
layout (location = 0) in vec3 coord;

uniform mat4 projection_view;
uniform mat4 model;

void main() {
    gl_Position = projection_view * model * vec4(coord, 1.0);
}  