#version 440 core

layout (location = 0) in uint vert_data;

uniform mat4 model;
uniform mat4 projection_view;

flat out uvec2 tex_coord;
out vec2 vert_pos;

void main() {
    uint x = bitfieldExtract(vert_data, 0, 5);
    uint y = bitfieldExtract(vert_data, 10, 9);
    uint z = bitfieldExtract(vert_data, 5, 5);
    uint uv_x = bitfieldExtract(vert_data, 19, 1);
    uint uv_y = bitfieldExtract(vert_data, 20, 1);
    uint tex_x = bitfieldExtract(vert_data, 21, 4);
    uint tex_y = bitfieldExtract(vert_data, 25, 4);

    tex_coord = uvec2(tex_x, 15 - tex_y);
    vert_pos = vec2(uv_x, uv_y);

    gl_Position = projection_view * model * vec4(x, y, z, 1.0f);
}