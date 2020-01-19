#version 330 core
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
out vec3 v2fWorldPos;
out vec2 uvCoord;
out vec3 v2fNormal;
out vec3 v2fTangent;
out vec3 v2fView;

void main()
{
	gl_Position = MVP * transform * vec4(pos, 1.0f);
	v2fColor = color;
	uvCoord = uv;
	v2fWorldPos = vec3(model * transform * vec4(pos, 1.0f));

	v2fNormal = normalize(vec3(model * transform * vec4(normal, 0.0)));
	v2fTangent = normalize(vec3(model * transform * vec4(tangent, 0.0)));

	// Can precalculate v2fView (camera position) as it is the same for all vertices (and inverse expensive)
	mat4 invView = inverse(view);
	v2fView = vec3(invView[3][0], invView[3][1], invView[3][2]);
}