#version 460 core

out vec4 FragColor;

in float vHeight;

void main()
{
float h = (vHeight)/32.0 + 1.0;
FragColor = vec4(h, h, h, 1.0);
}