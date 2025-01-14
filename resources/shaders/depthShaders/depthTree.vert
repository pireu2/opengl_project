#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=3) in vec3 instancePosition;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 model;
uniform sampler2D heightMap;
uniform float heightScale;

void main()
{
    vec2 gridSize = vec2(300.0f, 300.0f);
    vec3 offsetPos = vPosition + instancePosition;

    vec4 worldPosition = model * vec4(offsetPos, 1.0f);
    vec2 heightMapCoords = (vec2(worldPosition.x, -worldPosition.z) / gridSize) * 0.5f + 0.5f;
    float h = texture(heightMap, heightMapCoords).r * heightScale;

    vec3 newPosition = vec3(worldPosition.x, worldPosition.y + h, worldPosition.z);

    gl_Position = lightSpaceTrMatrix * vec4(newPosition, 1.0f);
}

