#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 projMat;
uniform mat4 viewMat;

void main()
{
vec4 pos;
vTexCoords = aPos;
pos = projMat * viewMat * vec4(aPos, 1.0);
gl_Position = pos.xyww;

}