#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//texture
uniform sampler2D treeTexture;
uniform sampler2D shadowMap;

out vec4 fColor;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;


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
}

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
    computeLightComponents();

    vec3 textureColor = texture(treeTexture, fTexCoords).rgb;
    float shadow = computeShadow();

    ambient *= textureColor;
    diffuse *= textureColor;
    specular *= textureColor;

    vec3 color = min((ambient + (1.0f - shadow) * diffuse) + specular, 1.0f);

    fColor = vec4(color, 1.0f);
}
