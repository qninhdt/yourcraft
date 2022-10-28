#version 440 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    // FragColor = texture(skybox, TexCoords);
    FragColor = vec4(1-abs(0.15/TexCoords.y), 0.9, 0.9, 1);
}