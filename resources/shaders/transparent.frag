#version 440 core

uniform sampler2D game_texture;

out vec4 FragColor;

flat in uvec2 tex_coord;
in vec2 vert_pos;

// uniform float zFar = 500.0f;
// uniform float zNear = 0.1f;

// layout(location = 0) out vec4 accumTexture;
// layout(location = 1) out vec4 revealageTexture;

// float d(float z) {
//     return ((zNear * zFar) / z - zFar) / (zNear - zFar);
// }

// float weight(float z, float a) {
//     float b = 1 - d(z);
//     return a * max(0.01f, b * b * b * 0.003);
// }

const float sprite_size = 1.0f / 16;

void main() {  
    vec2 coord = vec2(sprite_size * tex_coord.x, sprite_size * tex_coord.y);
    coord.x += (vert_pos.x) * sprite_size;
    coord.y += (vert_pos.y) * sprite_size;
    vec4 color = texture(game_texture, coord);
    FragColor = color;
    // accumTexture = color * weight(gl_FragCoord.z, color.w);
    // revealageTexture = vec4(color.w);
}
