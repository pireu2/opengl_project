#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D grassTexture;

void main()
{
    vec4 colorFromTexture = texture(grassTexture, fTexCoords);

    if(colorFromTexture.a < 0.5f)
    {
        discard;
    }

    fColor = colorFromTexture;
}
