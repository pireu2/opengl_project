#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform sampler2D heightMap;
uniform float heightScale;

vec2 groundSize = vec2(50.0f, 50.0f);

void main()
{
    vec4 worldPositon = model * vec4(vPosition, 1.0f);
    fPosEye = view * model * vec4(vPosition, 1.0f);
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;

    vec2 heightMapCoords = vec2(worldPositon.x / groundSize.x, worldPositon.z / groundSize.y) * 0.5f + 0.5f;
    float h = texture(heightMap, heightMapCoords).r * heightScale;

    vec3 newPosition = vec3(vPosition.x, vPosition.y + h, vPosition.z);

    gl_Position = projection * view * model * vec4(newPosition, 1.0f);
}
