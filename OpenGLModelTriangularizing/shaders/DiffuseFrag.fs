#version 330 core
in vec4 v2fCOLOR;
in vec3 v2fWorldPos;
in vec3 v2fNormal;
out vec4 fragColor;

uniform vec3 lightPos;

void main()
{
	float dotProd = max(dot(normalize(v2fNormal), normalize(lightPos - v2fWorldPos)), 0.0f);
	fragColor = dotProd * vec4(1, 1, 1, 1);
}