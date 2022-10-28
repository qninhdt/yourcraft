#version 440 core

uniform sampler2D game_texture;
uniform float zFar = 5000.0f;
uniform float zNear = 0.1f;

flat in uvec2 tex_coord;
in vec2 vert_pos;

layout (location = 0) out vec4 accum;
layout (location = 1) out float reveal;

const float sprite_size = 1.0f / 16;

float d(float z) {
    return ((zNear * zFar) / z - zFar) / (zNear - zFar);
}

float weight(float z, float a) {
    float b = 1 - d(z);
    return a * max(0.01f, b * b * b * 0.003);
}

void main() {  
    vec2 coord = vec2(sprite_size * tex_coord.x, sprite_size * tex_coord.y);
    coord.x += (vert_pos.x) * sprite_size;
    coord.y += (vert_pos.y) * sprite_size;

    vec4 color = texture(game_texture, coord);
    float w = weight(gl_FragCoord.z, color.w);

    if (color.w == 1) {
        w *= 10;
    }

    accum = vec4(color.xyz, color.w) * w;
    reveal = color.a;
}
