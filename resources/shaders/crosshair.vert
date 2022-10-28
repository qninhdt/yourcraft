#version 440 core
layout (location = 0) in vec2 coord;
layout (location = 1) in vec2 pass_texcoord;

uniform int screen_width;
uniform int screen_height;
uniform int width;
uniform int height;

out vec2 texcoord;

void main() {
    const float xsize = 1.0f*width/screen_width;
    const float ysize = 1.0f*height/screen_height;

    gl_Position = vec4(vec2(xsize*coord.x, ysize*coord.y), 0, 1.0);
    texcoord = pass_texcoord;
}  