#version 460 core
out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube skybox;

void main()
{    
FragColor = texture(skybox, vTexCoords);
}