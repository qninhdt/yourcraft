#version 440 core

in vec2 uv;

out vec4 FragColor;

const float outline_size = 0.015;

void main() {

    if (outline_size<uv.x && uv.x<1-outline_size &&
        outline_size<uv.y && uv.y<1-outline_size) {
        discard;
    }

    gl_FragDepth = gl_FragCoord.z - .00001;
    FragColor = vec4(236.0f/255, 240.0f/255, 240.0f/255, 1.0f);
}
