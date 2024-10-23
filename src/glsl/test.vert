#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aClr;
layout (location = 2) in vec2 aTexCoords;

out vec4 vClr;
out vec2 vTexCoords;

void main()
{
vClr = aClr;
vTexCoords = aTexCoords;
gl_Position = vec4( aPos, 1.0f );

}