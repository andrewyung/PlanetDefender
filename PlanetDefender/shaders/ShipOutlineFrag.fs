#version 330 core
in vec3 v2fWorldPos;
in vec2 uvCoord;
in vec3 v2fNormal;

uniform sampler2D mainTexture;
uniform int time;

uniform vec4 emit_color = vec4(1,1,1,1);
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
	fragColor = texture(mainTexture, uvCoord + (vec2(0.01f, 0.01f) * time));
    brightColor = emit_color * 0.2f;
}