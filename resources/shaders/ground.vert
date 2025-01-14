#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;

uniform sampler2D heightMap;
uniform float heightScale;

void main()
{
    vec2 texelSize = 1.0 / textureSize(heightMap, 0);
    float heightL = texture(heightMap, vTexCoords - vec2(texelSize.x, 0.0)).r * heightScale;
    float heightR = texture(heightMap, vTexCoords + vec2(texelSize.x, 0.0)).r * heightScale;
    float heightD = texture(heightMap, vTexCoords - vec2(0.0, texelSize.y)).r * heightScale;
    float heightU = texture(heightMap, vTexCoords + vec2(0.0, texelSize.y)).r * heightScale;

    vec3 normal = normalize(vec3(heightL - heightR, 2.0, heightD - heightU));



    float h = texture(heightMap, vTexCoords).r * heightScale;
    vec3 newPosition = vec3(vPosition.x, h, vPosition.z);

    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(newPosition, 1.0f);
    fPosEye = view * model * vec4(newPosition, 1.0f);
    fTexCoords = vTexCoords;
    fNormal = normalize(normalMatrix * normal);
    gl_Position = projection * view * model * vec4(newPosition, 1.0f);
}