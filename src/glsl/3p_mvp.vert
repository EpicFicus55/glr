#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec4 aClr;

out vec4 vClr;

void main()
{
vClr = aClr;
gl_Position = projMat * viewMat * modelMat * vec4( aPos, 1.0 );

}