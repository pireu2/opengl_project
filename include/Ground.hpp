#pragma once

#include <Model3D.hpp>
#include <Shader.hpp>
#include <glm/glm.hpp>

namespace gps
{
    class Ground
    {
    public:
        void init();
        void loadModel(const std::string &path);
        void loadShader(const std::string &vertexPath, const std::string &fragmentPath);
        void initUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix,
                          const glm::vec3 &lightDir, const glm::vec3 &lightColor);
        void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir, const glm::vec3 &lightColor,
                    const glm::mat4 &lightSpaceTrMatrix, unsigned int shadowMapTexture, const glm::vec3 &pointLightPosition, const glm::vec3 &pointLightColor);
        void render_depth(const glm::mat4 &lightSpaceTrMatrix);

    private:
        Model3D model;
        Shader shader = {};
        Shader depthMapShader = {};
        unsigned int heightMapTexture = 0;
        float heightScale = 30.0f;
    };
}