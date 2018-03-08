#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

uniform mat4 transform;
uniform int time;
uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 v2fCOLOR;
out vec3 v2fWorldPos;
out vec3 v2fNormal;

void main()
{
	gl_Position = MVP * transform * vec4(pos, 1.0f);
	v2fCOLOR = color;
	v2fWorldPos = vec3(model * transform * vec4(pos, 1.0f));
	v2fNormal = mat3(transpose(inverse(model * transform))) * normal;
}