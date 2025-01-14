#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

uniform	vec3 lightDir;
uniform	vec3 lightColor;

uniform sampler2D grassTexture;
uniform sampler2D shadowMap;

float computeShadow(){
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    normalizedCoords = 0.5f * normalizedCoords + 0.5f;
    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
    float currentDepth = normalizedCoords.z;
    float bias = max(0.001f * (1.0f - dot(fNormal, normalize(lightDir))), 0.0001f);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;


    if(normalizedCoords.z > 1.0f){
        shadow = 0.0f;
    }

    return shadow;
}

void main()
{
    vec4 colorFromTexture = texture(grassTexture, fTexCoords);

    if(colorFromTexture.a < 0.5f)
    {
        discard;
    }

    float shadow = computeShadow();
    vec3 color = colorFromTexture.rgb * (1.0f - 0.8f * shadow);

    fColor = vec4(color, 1.0f);
}
