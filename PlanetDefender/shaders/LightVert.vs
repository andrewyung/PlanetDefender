#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec3 tangent;

uniform mat4 transform;
uniform int time;
uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 v2fColor;

void main()
{
	gl_Position = MVP * transform * vec4(pos, 1.0f);
	v2fColor = color;
}