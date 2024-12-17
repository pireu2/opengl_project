#version 410 core

in vec3 fFragPos;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

uniform vec3 lightPos; // Light position in world space
uniform vec3 viewPos;  // Camera position in world space
uniform vec3 lightColor;

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

uniform float ambientStrength;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform float shininess;
uniform vec3 specularColor;
uniform float mixStrength;

uniform vec3 tipColor;
uniform float tipAttenuation;

uniform samplerCube skyboxTex;

uniform float fresnelNormalStrength;
uniform float fresnelShininess;
uniform float fresnelBias;
uniform float fresnelStrength;
uniform vec3 fresnelColor;

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

void computeLight(vec3 normal, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
    ambient = ambientStrength * ambientColor;

    vec3 lightDir = normalize(lightPos - fFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = diff * diffuseColor;

    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    specular = specularColor * spec;
}

void main()
{
    vec3 fbmResult = fragmentFBM(fFragPos);
    vec3 normal = normalize(vec3(fbmResult.y, 1.0, fbmResult.z));

    vec3 ambient, diffuse, specular;
    computeLight(normal, ambient, diffuse, specular);

    vec3 tip = tipColor * pow(fbmResult.x, tipAttenuation);

    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 reflectedDir = reflect(-viewDir, normal);
    vec3 skyboxColor = texture(skyboxTex, reflectedDir).rgb;

    vec3 fresnelNormal = normal;
    fresnelNormal.xz *= fresnelNormalStrength;
    fresnelNormal = normalize(fresnelNormal);
    float base = 1.0 - dot(viewDir, fresnelNormal);
    float exponential = pow(base, fresnelShininess);
    float R = exponential + fresnelBias * (1.0 - exponential);
    R *= fresnelStrength;
    vec3 fresnel = fresnelColor * R;

    // Apply Fresnel effect to specular and mix with skybox color
    vec3 finalColor = ambient + R * (diffuse + specular) + tip;
    finalColor = mix(finalColor, skyboxColor, mixStrength * R);

    fColor = vec4(finalColor, 1.0);
}