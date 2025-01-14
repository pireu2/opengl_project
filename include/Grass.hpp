#pragma once

#include <InstancedObject.hpp>

namespace gps
{

    class Grass : public InstancedObject
    {
    public:
        void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir, const glm::mat4 &lightSpaceTrMatrix, unsigned int shadowMapTexture);
        void drawImguiControls() override;
        void init();
        void initUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3& lightDir, const glm::vec3& lightColor);
        std::vector<glm::vec3> generateGrassPositions();

    private:
        int gridSize = 600;
        float spacing = 1.0f;
        float radius = 160.0f;
        float heightScale = 30.0f;
        float windStrength = 0.5f;
        unsigned int heightMapTexture = 0;
        unsigned int grassTexture = 0;
    };

}