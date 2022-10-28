#version 440 core

uniform sampler2D crosshair_texture;

in vec2 texcoord;

out vec4 FragColor;

void main() {
    FragColor = texture(crosshair_texture, texcoord);
}  