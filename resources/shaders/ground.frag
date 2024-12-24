#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;

out vec4 fColor;

//lighting
uniform	vec3 lightPos;
uniform	vec3 lightColor;

//texture
uniform sampler2D diffuseTexture;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;



void computeLightComponents()
{
    vec3 cameraPosEye = vec3(0.0f);

    vec3 lightDir = lightPos - fPosEye.xyz;

    vec3 normalEye = normalize(fNormal);

    vec3 lightDirN = normalize(lightDir);

    vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

    vec3 reflection = reflect(-lightDirN, normalEye);


    ambient = ambientStrength * lightColor;
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;
}



void main()
{
    computeLightComponents();

    vec3 textureColor = texture(diffuseTexture, fTexCoords).rgb;

    ambient *= textureColor;
    diffuse *= textureColor;
    specular *= textureColor;

    vec3 color = min((ambient + diffuse) + specular, 1.0f);

    fColor = vec4(color, 1.0f);

}
