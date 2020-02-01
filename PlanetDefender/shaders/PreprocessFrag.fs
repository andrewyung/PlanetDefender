#version 420 core
in vec2 uvCoord;
out vec4 fragColor;

layout(binding = 0) uniform sampler2D mainTexture;
layout(binding = 1) uniform sampler2D brightTexture;


void main()
{
    fragColor = texture(mainTexture, uvCoord);
}