#version 460 core

in  vec4 vClr;
in  vec2 vTexCoords;

uniform sampler2D albedoTex;

out vec4 FragColor;

void main()
{
FragColor = texture(albedoTex, vTexCoords);

}