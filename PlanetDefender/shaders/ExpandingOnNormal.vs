#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

uniform mat4 transform;
uniform int time;
uniform mat4 MVP;

out vec4 v2fCOLOR;

void main()
{
	//just multiplies vertex, will vary greatly depending on what transformations have been done on transform
	//currently doesnt work with rotation transformation in mvp
	gl_Position = MVP * transform  * (vec4((normal * (1f - cos(time / 250f)) * 0.4f) + pos, 1.0f));
	v2fCOLOR = vec4(normal, 1.0);
}