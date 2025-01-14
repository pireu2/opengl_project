#pragma once

#include <Model3D.hpp>
#include <Shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gps
{

    class Water
    {
    public:
        void initUniforms(glm::mat4 modelMatrix, glm::mat4 view, glm::mat3 normalMatrix, glm::vec3 lightDir,
                          glm::vec3 lightColor, glm::vec3 viewPos);

        void drawImguiControls();

        void render(glm::mat4 model,glm::mat4 view, glm::mat4 projection,glm::mat3 normalMatrix, glm::vec3 lightDir, glm::vec3 viewPos);

        void loadModel(const std::string &fileName);

        void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);

    private:
        Model3D model;
        Shader shader = {};

        glm::vec3 specularColor = glm::vec3(226.0f / 255.0f, 243.0f / 255.0f, 160.0f / 255.0f);
        glm::vec3 ambientColor = glm::vec3(0.0f, 67.0f / 255.0f, 120.0f / 255.0f);
        glm::vec3 diffuseColor = glm::vec3(0.0f, 102.0f / 255.0f, 174.0f / 255.0f);
        glm::vec3 tipColor = glm::vec3(1.0f, 1.0f, 1.0f);

        float ambientStrength = 0.365f;
        float shininess = 68.0f;
        float tipAttenuation = 5.0f;
        float mixStrength = 0.8f;

        float fresnelNormalStrength = 0.2f;
        float fresnelShininess = 5.0f;
        float fresnelBias = 0.4f;
        float fresnelStrength = 0.5f;

        int vertexWaveCount = 8;
        int fragmentWaveCount = 40;

        float vertexSeed = 4.0f;
        float vertexSeedIter = 4.3f;
        float vertexFrequency = 1.0f;
        float vertexFrequencyMult = 1.16f;
        float vertexAmplitude = 1.0f;
        float vertexAmplitudeMult = 0.83f;
        float vertexInitialSpeed = 2.0f;
        float vertexSpeedRamp = 1.07f;
        float vertexDrag = 0.5f;
        float vertexHeight = 1.48f;
        float vertexMaxPeak = 1.0f;
        float vertexPeakOffset = 1.0f;

        float fragmentSeed = 4.0f;
        float fragmentSeedIter = 4.3f;
        float fragmentFrequency = 1.0f;
        float fragmentFrequencyMult = 1.16f;
        float fragmentAmplitude = 1.0f;
        float fragmentAmplitudeMult = 0.83f;
        float fragmentInitialSpeed = 2.0f;
        float fragmentSpeedRamp = 1.07f;
        float fragmentDrag = 0.5f;
        float fragmentHeight = 1.34f;
        float fragmentMaxPeak = 1.0f;
        float fragmentPeakOffset = 1.14f;
    };
}
