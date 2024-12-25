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

void main()
{
    // Compute eye space coordinates
    fPosEye = view * model * vec4(vPosition, 1.0f);
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;

    // Get height from heightmap and scale it
    float h = texture(heightMap, vTexCoords).r * heightScale;

    // Apply height to the vertex position
    vec3 newPosition = vec3(vPosition.x, h, vPosition.z);
    gl_Position = projection * view * model * vec4(newPosition, 1.0f);
}