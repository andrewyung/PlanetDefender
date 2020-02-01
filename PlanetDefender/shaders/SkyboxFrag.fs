#version 330 core
in vec2 uvCoord;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D texture0;

void main()
{
    fragColor = texture(texture0, uvCoord);
    brightColor = vec4(0, 0, 0, 1);
}