#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;
out vec3 lightPosEye;

uniform vec3 pointLightPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;


void main()
{
    //compute eye space coordinates
    fPosEye = view * model * vec4(vPosition, 1.0f);
    lightPosEye = vec3(view * vec4(pointLightPosition, 1.0f));
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
