#version 330 core
in vec4 v2fColor;
in vec3 v2fWorldPos;
in vec2 uvCoord;
in vec3 v2fNormal;
out vec4 fragColor;

#define MAX_LIGHTS 9
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform sampler2D mainTexture;
uniform sampler2D normalTexture;

void main()
{
	vec4 color = vec4(0, 0, 0, 1);
	float attenuation;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		attenuation = 1.0f / (length(v2fWorldPos - lightPos[i].xyz));
		float dotProd = max(dot(normalize(v2fNormal), normalize(v2fWorldPos - lightPos[i].xyz)), 0.0f);
		color += lightPos[i].w * (dotProd * attenuation * vec4(lightColor[i].xyz, 1));
	}

	fragColor = texture(mainTexture, uvCoord) + color;
}