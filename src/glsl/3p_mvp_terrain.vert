#version 460 core
layout (location = 0) in vec3 aPos;

out float vHeight;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
vHeight = aPos.y;
gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}