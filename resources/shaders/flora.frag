#version 440 core

out vec4 FragColor;

uniform sampler2D game_texture;

in vec2 uv;
flat in uvec2 texcoord;

const float sprite_size = 1.0f / 16;

void main() {  
    vec2 coord = vec2(sprite_size * texcoord.x, sprite_size * texcoord.y);
    coord.x += (uv.x) * sprite_size;
    coord.y += (uv.y) * sprite_size;
    FragColor = texture(game_texture, coord);

    if (FragColor.w == 0) {
        discard;
    }
}
