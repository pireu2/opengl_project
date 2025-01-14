#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=2) in vec2 vTexCoords;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 model;
uniform sampler2D heightMap;
uniform float heightScale;

void main()
{
    float h = texture(heightMap, vTexCoords).r * heightScale;
    vec3 newPosition = vec3(vPosition.x, h, vPosition.z);

    gl_Position = lightSpaceTrMatrix * model * vec4(newPosition, 1.0f);
}

