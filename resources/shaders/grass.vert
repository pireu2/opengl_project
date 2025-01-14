#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;
layout(location=3) in vec3 instancePosition;

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

uniform float time;
uniform float windStrength;

void main()
{
    vec2 gridSize = vec2(300.0f, 300.0f);
    vec3 offsetPos = vPosition + instancePosition;

    vec4 worldPosition = model * vec4(offsetPos, 1.0f);
    vec2 heightMapCoords = (vec2(worldPosition.x, -worldPosition.z) / gridSize) * 0.5f + 0.5f;
    float h = texture(heightMap, heightMapCoords).r * heightScale;



    vec3 newPosition = vec3(offsetPos.x, offsetPos.y + h, offsetPos.z);
    float windEffect = sin(time + instancePosition.x * 0.1 + instancePosition.z * 0.1) * windStrength;
    if (vPosition.y > 0.5) {
        newPosition.x += windEffect;
        newPosition.z += windEffect;
    }

    fPosEye = view * model * vec4(newPosition, 1.0f);
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(newPosition, 1.0f);

    gl_Position = projection * view * model * vec4(newPosition, 1.0f);
}