#version 330 core

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
	fragColor = vec4(0,1,0,1);
    brightColor = vec4(0,0,0,0);
}