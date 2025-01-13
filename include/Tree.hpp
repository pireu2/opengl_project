#include <InstancedObject.hpp>

namespace gps
{
    class Tree : public InstancedObject
    {
    public:
        void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir);
        void drawImguiControls() override;
        void init();
        void initUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir, const glm::vec3 &lightColor);
        std::vector<glm::vec3> generateTreePositions();

    private:
        int gridSize = 300;
        float spacing = 5.0f;
        float radius = 75.0f;
        float heightScale = 30.0f;
        float windStrength = 0.5f;
        unsigned int heightMapTexture = 0;
        unsigned int treeTexture = 0;
    };
}