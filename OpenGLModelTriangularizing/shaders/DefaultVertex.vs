#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;

uniform mat4 transform;
uniform int time;

out vec4 v2fCOLOR;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
uniform int time;
}