#version 420 core
in vec4 v2fColor;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform vec4 emit_color = vec4(1,1,1,1);

void main()
{
    fragColor = emit_color;
    brightColor = emit_color;
}