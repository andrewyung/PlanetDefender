#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

uniform mat4 transform;
uniform int time;

out vec4 v2fCOLOR;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
	v2fCOLOR = color * sin(time / 150f);
}