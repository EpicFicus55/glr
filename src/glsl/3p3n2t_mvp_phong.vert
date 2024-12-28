#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

uniform mat4 uNormalMat;

out vec3 vFragPos;
out vec3 vNorm;
out vec2 vTexCoords;

void main()
{
vFragPos = vec3(modelMat * vec4(aPos, 1.0));
vNorm = mat3(uNormalMat) * aNorm;
vTexCoords = aTexCoords;
gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

}