#include <Grass.hpp>

namespace gps{

    void Grass::init()
    {
        heightMapTexture = Model3D::ReadTextureFromFile(RESOURCES_PATH "textures/heightmap.png");
        grassTexture = Model3D::ReadTextureFromFile(RESOURCES_PATH "textures/grass.png", 1);
    }

    void Grass::initUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix)
    {
        shader.useShaderProgram();
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
    }

    void Grass::drawImguiControls()
    {
        ImGui::Begin("Grass Controls");
        ImGui::DragFloat("Height Scale", &heightScale, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat("Height Threshold", &heightThreshold, 1.0f, 0.0f, 10000.0f);
        ImGui::DragFloat("Hard Threshold", &hardThreshold, 0.1f, 0.0f, 100.0f);
        ImGui::DragFloat("Wind Strength", &windStrength, 0.01f, 0.0f, 5.0f);
        ImGui::End();
    }

    void Grass::setInstancePositions(const std::vector<glm::vec3> &instancePositions)
    {
        this->instancePositions = instancePositions;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instancePositions.size() * sizeof(glm::vec3), &instancePositions[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Grass::render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix)
    {
        glDisable(GL_CULL_FACE);
        shader.useShaderProgram();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
        shader.setFloat("heightScale", heightScale);
        shader.setFloat("heightThreshold", heightThreshold);
        shader.setFloat("hardThreshold", hardThreshold);
        shader.setFloat("time", static_cast<float>(glfwGetTime()));
        shader.setFloat("windStrength", windStrength);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightMapTexture);
        shader.setInt("heightMap", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        shader.setInt("grassTexture", 2);

        glBindVertexArray(model.getVAO());
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(3, 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, model.getVertexCount(), instancePositions.size());
        glBindVertexArray(0);

        glEnable(GL_CULL_FACE);
    }

    std::vector<glm::vec3> Grass::generateGrassPositions()
    {
        std::vector<glm::vec3> positions;
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(-spacing / 2.0f, spacing / 2.0f);

        for (int x = -gridSize / 2; x < gridSize / 2; ++x)
        {
            for (int z = -gridSize / 2; z < gridSize / 2; ++z)
            {
                float offsetX = distribution(generator);
                float offsetZ = distribution(generator);
                positions.emplace_back(x * spacing + offsetX, 0.0f, z * spacing + offsetZ);
            }
        }
        return positions;

     }

}