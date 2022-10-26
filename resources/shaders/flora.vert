#version 440 core

layout (location = 0) in vec3 coord;
layout (location = 1) in vec2 pass_uv;
layout (location = 2) in uint texcoord_data;

uniform mat4 model;
uniform mat4 projection_view;

out vec2 uv;
flat out uvec2 texcoord;

void main()
{
    texcoord.x = bitfieldExtract(texcoord_data, 0, 4);
    texcoord.y = bitfieldExtract(texcoord_data, 4, 4);
    texcoord.y = 15 - texcoord.y;
    gl_Position = projection_view * model * vec4(coord, 1.0);
    uv = pass_uv;
}  