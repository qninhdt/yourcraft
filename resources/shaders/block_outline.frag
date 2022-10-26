#version 440 core

out vec4 FragColor;

void main() {  
    gl_FragDepth = gl_FragCoord.z - .0001;
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
