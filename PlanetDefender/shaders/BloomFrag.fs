#version 420 core
in vec2 uvCoord;
out vec4 fragColor;

layout(binding = 0) uniform sampler2D brightTexture;

uniform bool horizontal;
uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

void main()
{
    fragColor = texture2D(brightTexture, uvCoord) * weight[0];
    if(horizontal)
    {
        for (int i = 1; i < 3; i++) 
        {
            fragColor += texture2D(brightTexture, uvCoord + vec2(0.0, offset[i] / 900)) * weight[i];
            fragColor += texture2D(brightTexture, uvCoord - vec2(0.0, offset[i] / 900)) * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 3; i++) 
        {
            fragColor += texture2D(brightTexture, uvCoord + vec2(offset[i] / 1600, 0.0)) * weight[i];
            fragColor += texture2D(brightTexture, uvCoord - vec2(offset[i] / 1600, 0.0)) * weight[i];
        }
    }
}