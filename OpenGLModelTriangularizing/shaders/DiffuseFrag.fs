#version 330 core
in vec4 v2fCOLOR;
in vec3 v2fWorldPos;
in vec3 v2fNormal;
out vec4 fragColor;

#define MAX_LIGHTS 9
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];

void main()
{
	float attenuation = 1 / length(v2fWorldPos - lightPos[0].xyz);
	float dotProd = max(dot(normalize(v2fNormal), normalize(v2fWorldPos - lightPos[0].xyz)), 0.0f);
	fragColor = lightPos[0].w * dotProd * attenuation * vec4(lightColor[0].xyz, 1);
	
	attenuation = 1 / length(v2fWorldPos - lightPos[1].xyz);
	dotProd = max(dot(normalize(v2fNormal), normalize(v2fWorldPos - lightPos[1].xyz)), 0.0f);
	fragColor = lightPos[1].w * dotProd * attenuation * vec4(lightColor[1].xyz, 1) + vec4(0.1f, 0.1f, 0.1f, 1) + fragColor;
}