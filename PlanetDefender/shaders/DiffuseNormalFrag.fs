#version 420 core
in vec4 v2fColor;
in vec3 v2fWorldPos;
in vec2 uvCoord;
in vec3 v2fNormal;
in vec3 v2fTangent;
in vec3 v2fView;

#define MAX_LIGHTS 9
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec4 ambientLight;
uniform float shininess = 1;
layout(binding = 0) uniform sampler2D mainTexture;
layout(binding = 1) uniform sampler2D normalTexture;
layout(binding = 2) uniform sampler2D specularMap;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
    // Tangents for TBN matrix
    vec3 tangent = normalize(v2fTangent - dot(v2fTangent, v2fNormal) * v2fNormal);
    vec3 biTangent = cross(tangent, normalize(v2fNormal));

    // Calculate normal with respect to normal map
    vec3 normMapVec = texture(normalTexture, uvCoord).rgb;
    normMapVec = normMapVec * 2.0 - 1.0; // convert from [0,1] to [-1,1]
     
    // tangent to world space matrix
    mat3 TBN = transpose(mat3(tangent, biTangent, normalize(v2fNormal)));

    normMapVec = normMapVec * TBN;
    normMapVec = normalize(normMapVec);
        
    // Specular
    vec3 viewDir = normalize(v2fView - v2fWorldPos);
    vec4 specularMap = texture(specularMap, uvCoord);

    // Combine from light sources
	vec4 color = vec4(0, 0, 0, 0);
	float attenuation;
    vec3 specular = vec3(0, 0, 0);
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
        vec3 lightDir = lightPos[i].xyz - v2fWorldPos;
        vec3 reflectDir = reflect(-normalize(lightDir), normMapVec);
        if (dot(normMapVec, -normalize(lightDir)) > 0)
        {
            specular += pow(max(dot(viewDir, reflectDir), 0.0), 32) * specularMap.xyz * lightColor[i].xyz * shininess;
        }

        attenuation = 1.0f / pow(length(lightDir), 2);
		float dotProd = abs(dot(normMapVec, normalize(lightDir)));
		color += lightPos[i].w * (dotProd * attenuation * vec4(lightColor[i].xyz, dotProd));
    }
   
    //fragColor = vec4(specular, 1);
	fragColor = (texture(mainTexture, uvCoord) + ambientLight + color + vec4(specular, 1)) * (color.w + ambientLight.w);

    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightColor = vec4(fragColor.rgb, 1.0);
    else
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}