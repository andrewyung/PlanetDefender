#version 420 core
in vec4 v2fColor;
in vec3 v2fWorldPos;
in vec2 uvCoord;
in vec3 v2fNormal;
in vec3 v2fTangent;
out vec4 fragColor;

#define MAX_LIGHTS 9
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float ambientLight;
layout(binding = 0) uniform sampler2D mainTexture;
layout(binding = 1) uniform sampler2D normalTexture;

void main()
{
    vec3 tangent = normalize(v2fTangent - dot(v2fTangent, v2fNormal) * v2fNormal);
    vec3 biTangent = cross(tangent, normalize(v2fNormal));
    
    vec3 normMapVec = texture(normalTexture, uvCoord).rgb;
    normMapVec = normMapVec * 2.0 - 1.0; // convert from [0,1] to [-1,1]
     
    // tangent to world space matrix
    mat3 TBN = transpose(mat3(tangent, biTangent, normalize(v2fNormal)));

    normMapVec = normMapVec * TBN;
    normMapVec = normalize(normMapVec);
        
	vec4 color = vec4(0, 0, 0, 0);
	float attenuation;
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
        attenuation = 1.0f / (length((v2fWorldPos - lightPos[i].xyz)));
		float dotProd = max(dot(normMapVec, normalize((v2fWorldPos - lightPos[i].xyz))), 0.0);
		color += lightPos[i].w * (dotProd * attenuation * vec4(lightColor[i].xyz, dotProd));
    }

	fragColor = (texture(mainTexture, uvCoord) + color) * (color.w + ambientLight);
}