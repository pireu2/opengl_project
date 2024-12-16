#version 410 core

in vec3 fFragPos;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

uniform vec3 lightPos; // Light position in world space
uniform vec3 viewPos;  // Camera position in world space
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float time;
uniform int fragmentWaveCount;
uniform float fragmentSeed;
uniform float fragmentSeedIter;
uniform float fragmentFrequency;
uniform float fragmentFrequencyMult;
uniform float fragmentAmplitude;
uniform float fragmentAmplitudeMult;
uniform float fragmentInitialSpeed;
uniform float fragmentSpeedRamp;
uniform float fragmentDrag;
uniform float fragmentHeight;
uniform float fragmentMaxPeak;
uniform float fragmentPeakOffset;

const float pi = 3.14159265359;

vec3 fragmentFBM(vec3 v) {
    float f = fragmentFrequency;
    float a = fragmentAmplitude;
    float speed = fragmentInitialSpeed;
    float seed = fragmentSeed;
    vec3 p = v;

    float h = 0.0f;
    vec2 n = vec2(0.0f);

    float amplitudeSum = 0.0f;

    for (int wi = 0; wi < fragmentWaveCount; ++wi) {
        vec2 d = normalize(vec2(cos(seed), sin(seed)));

        float x = dot(d, p.xz) * f + time * speed;
        float wave = a * exp(fragmentMaxPeak * sin(x) - fragmentPeakOffset);
        vec2 dw = f * d * (fragmentMaxPeak * wave * cos(x));

        h += wave;
        p.xz += -dw * a * fragmentDrag;

        n += dw;

        amplitudeSum += a;
        f *= fragmentFrequencyMult;
        a *= fragmentAmplitudeMult;
        speed *= fragmentSpeedRamp;
        seed += fragmentSeedIter;
    }

    vec3 result = vec3(h, n.x, n.y) / amplitudeSum;
    result.x *= fragmentHeight;

    return result;
}

void computeLightComponents(vec3 normal, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
    // Ambient lighting
    float ambientStrength = 0.2f;
    ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - fFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5f;
    float shininess = 32.0f;
    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular = specularStrength * spec * lightColor;
}

void main()
{
    vec3 fbmResult = fragmentFBM(fFragPos);
    vec3 normal = normalize(vec3(fbmResult.y, 1.0, fbmResult.z));

    vec3 ambient, diffuse, specular;
    computeLightComponents(normal, ambient, diffuse, specular);

    // Combine results
    vec3 result = ambient + diffuse + specular;
    fColor = vec4(result * objectColor, 1.0);
}