#version 460 core

in vec2 vTexCoords;

uniform sampler2D screenTexture;

out vec4 FragColor;

void main()
{
FragColor = texture(screenTexture, vTexCoords);
}