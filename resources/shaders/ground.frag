#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec3 lightPosEye;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D heightMap;
uniform sampler2D shadowMap;
uniform float heightScale;

uniform vec3 pointLightColor;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

float constant = 1.0f;
float linear = 0.007f;
float quadratic = 0.0002f;



void computeLightComponents()
{
    vec3 cameraPosEye = vec3(0.0f);

    vec3 normalEye = normalize(fNormal);

    vec3 lightDirN = normalize(lightDir);

    vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

    vec3 reflection = reflect(-lightDirN, normalEye);


    ambient = ambientStrength * lightColor;
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;

    vec3 lightDirNPoint = normalize(lightPosEye - fPosEye.xyz);
    //compute distance to light
    float dist = length(lightPosEye - fPosEye.xyz);
    //compute attenuation
    float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));

    vec3 ambientPoint = att * ambientStrength * pointLightColor;
    vec3 diffusePoint = att * max(dot(normalEye, lightDirNPoint), 0.0f) * pointLightColor;
    vec3 specularPoint = att * specularStrength * specCoeff * pointLightColor;

    ambient += ambientPoint;
    diffuse += diffusePoint;
    specular += specularPoint;
}

float computeShadow(){
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    normalizedCoords = 0.5f * normalizedCoords + 0.5f;

    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
    float currentDepth = normalizedCoords.z;
    float bias = max(0.005f * (1.0f - dot(fNormal, normalize(lightDir))), 0.005f);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

    if(normalizedCoords.z > 1.0f){
        shadow = 0.0f;
    }

    return shadow;

}

void main()
{
    computeLightComponents();

    vec3 textureColor = texture(diffuseTexture, fTexCoords).rgb;

    float height = texture(heightMap, fTexCoords).r * heightScale;

    vec3 yellow = vec3(1.0f, 1.0f, 0.65f);

    float blendFactor = clamp(1.0f - height + 0.2f, 0.0f, 1.0f);
    vec3 blendedColor = mix(textureColor, yellow, blendFactor);

    ambient *= blendedColor;
    diffuse *= blendedColor;
    specular *= blendedColor;

    float shadow = computeShadow();

    vec3 color = min((ambient + (1.0f - shadow) * diffuse) + specular, 1.0f);


    fColor = vec4(color, 1.0f);
}
