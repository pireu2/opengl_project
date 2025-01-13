#pragma once

#include <InstancedObject.hpp>

namespace gps {

    class Grass : public InstancedObject {
    public:
        void setInstancePositions(const std::vector<glm::vec3> &positions) override;
        void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix) override;
        void drawImguiControls() override;
        void init();
        void initUniforms(const glm::mat4 &model,const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix);
        std::vector<glm::vec3> generateGrassPositions();

    private:

        int gridSize = 600;
        float spacing = 1.0f;
        float heightScale = 30.0f;
        float heightThreshold = 300.0f;
        float hardThreshold = 1.3f;
        float windStrength = 0.5f;
        unsigned int heightMapTexture = 0;
        unsigned int grassTexture = 0;
    };

}