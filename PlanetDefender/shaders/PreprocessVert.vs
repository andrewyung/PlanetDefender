#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

uniform mat4 transform;

out vec2 uvCoord;

void main()
{    	
	gl_Position = transform * vec4(pos, 1.0f);

	uvCoord = uv;
}