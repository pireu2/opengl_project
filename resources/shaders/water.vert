#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fFragPos;
out vec2 fTexCoords;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform float time;
uniform int vertexWaveCount;
uniform float vertexSeed;
uniform float vertexSeedIter;
uniform float vertexFrequency;
uniform float vertexFrequencyMult;
uniform float vertexAmplitude;
uniform float vertexAmplitudeMult;
uniform float vertexInitialSpeed;
uniform float vertexSpeedRamp;
uniform float vertexDrag;
uniform float vertexHeight;
uniform float vertexMaxPeak;
uniform float vertexPeakOffset;

const float pi = 3.14159265359;


vec3 vertexFBM(vec3 v) {
    float f = vertexFrequency;
    float a = vertexAmplitude;
    float speed = vertexInitialSpeed;
    float seed = vertexSeed;
    vec3 p = v;
    float amplitudeSum = 0.0f;

    float h = 0.0f;
    vec2 n = vec2(0.0f);
    for (int wi = 0; wi < vertexWaveCount; ++wi) 
    {
        vec2 d = normalize(vec2(cos(seed), sin(seed)));

        float x = dot(d, p.xz) * f + time * speed;
        float wave = a * exp(vertexMaxPeak * sin(x) - vertexPeakOffset);
        float dx = vertexMaxPeak * wave * cos(x);

        h += wave;

        p.xz += d * -dx * a * vertexDrag;

        amplitudeSum += a;
        f *= vertexFrequencyMult;
        a *= vertexAmplitudeMult;
        speed *= vertexSpeedRamp;
        seed += vertexSeedIter;
    }

    vec3 result = vec3(h, n.x, n.y) / amplitudeSum;
    result.x *= vertexHeight;

    return result;
}



void main()
{
    vec3 position = vPosition;

    vec3 fbmResult = vertexFBM(position);
    position.y += fbmResult.x;

    fFragPos = vec3(model * vec4(position, 1.0));
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;

    gl_Position = projection * view * vec4(position, 1.0);
}