#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D depthTexture;
uniform samplerCube skyboxTex;

uniform float fogDensity;
uniform float fogOffset;
uniform vec3 fogColor;
uniform float fogHeight;
uniform float fogAttenuation;
uniform float skyboxSpeed;
uniform vec3 skyboxDirection;

uniform float distanceFog;

uniform mat4 cameraInvViewProjection;
uniform vec3 cameraPos;
uniform float time;


vec3 computeWorldSpacePosition(vec2 uv, float depth)
{
    vec4 positionCS = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 hpositionWS = cameraInvViewProjection * positionCS;
    return hpositionWS.xyz / hpositionWS.w;
}

vec4 flowUVW(vec3 dir, vec3 curl, float t, bool flowB)
{
    float phaseOffset = flowB ? 0.5f : 0.0f;
    float progress = t + phaseOffset - floor(t + phaseOffset);
    vec3 offset = curl * progress;

    vec4 uvw = vec4(dir, 0.0f);
    uvw.xz -= offset.xy;
    uvw.w = 1.0f - abs(1.0f - 2.0f * progress);

    return uvw;
}

void main()
{
    vec4 sceneColor = texture(scene, TexCoords);
    float depth = texture(depthTexture, TexCoords).r;
    vec3 worldPos = computeWorldSpacePosition(TexCoords, depth);
    vec3 viewDir = normalize(cameraPos - worldPos);

    if (depth >= 1.0)
    {
        vec3 curl = normalize(skyboxDirection);
        float t = time * skyboxSpeed;

        vec4 uvw1 = flowUVW(-viewDir, curl, t, false);
        vec4 uvw2 = flowUVW(-viewDir, curl, t, true);

        vec3 sky = texture(skyboxTex, uvw1.xyz).rgb * uvw1.w;
        vec3 sky2 = texture(skyboxTex, uvw2.xyz).rgb * uvw2.w;

        sceneColor.rgb = (sky + sky2);
    }

    float height = min(fogHeight, worldPos.y) / fogHeight;
    height = pow(clamp(height, 0.0, 1.0), 1.0f / fogAttenuation);

    float viewDistance = depth * distanceFog;

    float fogFactor = (fogDensity / sqrt(log(2.0))) * max(0.0, viewDistance - fogOffset);
    fogFactor = exp2(-fogFactor * fogFactor);

    vec3 result = mix(fogColor, sceneColor.rgb, clamp(height + fogFactor, 0.0, 1.0));

    FragColor = vec4(result, 1.0);
}