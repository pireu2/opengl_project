#pragma once

#include <Shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>

namespace gps{

    class Atmosphere
    {
    public:
        void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
        void setUniforms(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition, float time);
        void setSceneTexture(int textureId);
        void setDepthTexture(int textureId);
        void setSkyboxTexture(int textureId);
        void drawImguiControls();
    private:
        Shader shader = {};

        float fogDensity = 0.5f;
        float fogOffset = 3557.0f * 2;
        float fogHeight = 500.0f;

        float fogAttenuation = 1.0f;
        glm::vec3 fogColor = glm::vec3(1.0f, 0.95f, 0.9f);
        float distanceFog = 3560.0f * 2;

        glm::vec3 sunColor = glm::vec3(1.0f, 0.7f, 0.0f);
        glm::vec3 sunDirection = glm::vec3(-0.024f, -0.062f, 1.0f);


        float skyboxSpeed = 0.05f;
        glm::vec3 skyboxDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    };
}
