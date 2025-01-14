#include <Tree.hpp>

namespace gps
{
    void Tree::init()
    {
        heightMapTexture = Model3D::ReadTextureFromFile(RESOURCES_PATH "textures/heightmap.png");
        treeTexture = Model3D::ReadTextureFromFile(RESOURCES_PATH "objects/palm/palm.png");
        depthMapShader.loadShader(RESOURCES_PATH "shaders/depthShaders/depthTree.vert", RESOURCES_PATH "shaders/depthShaders/depthMap.frag");
    }

    void Tree::initUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir, const glm::vec3 &lightColor)
    {
        shader.useShaderProgram();
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
        shader.setVec3("lightDir", glm::value_ptr(lightDir));
        shader.setVec3("lightColor", glm::value_ptr(lightColor));

        depthMapShader.useShaderProgram();
        depthMapShader.setMat4("model", glm::value_ptr(model));
    }

    void Tree::drawImguiControls()
    {
        ImGui::Begin("Tree Controls");
        ImGui::SliderFloat("Wind Strength", &windStrength, 0.0f, 1.0f);
        ImGui::End();
    }

    void Tree::render(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat3 &normalMatrix,
            const glm::vec3 &lightDir, const glm::vec3 &lightColor, const glm::mat4 &lightSpaceTrMatrix,
            unsigned int shadowMapTexture, glm::vec3 pointLightPosition, glm::vec3 pointLightColor)
    {
        shader.useShaderProgram();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
        shader.setVec3("lightDir", glm::value_ptr(lightDir));
        shader.setVec3("lightColor", glm::value_ptr(lightColor));
        shader.setMat4("lightSpaceTrMatrix", glm::value_ptr(lightSpaceTrMatrix));
        shader.setVec3("pointLightPosition", glm::value_ptr(pointLightPosition));
        shader.setVec3("pointLightColor", glm::value_ptr(pointLightColor));
        shader.setFloat("heightScale", heightScale);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightMapTexture);
        shader.setInt("heightMap", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, treeTexture);
        shader.setInt("treeTexture", 2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        shader.setInt("shadowMap", 3);

        glBindVertexArray(model.getVAO());
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(3, 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, model.getVertexCount(), instancePositions.size());
        glBindVertexArray(0);
    }

    void Tree::render_depth(const glm::mat4 &lightSpaceTrMatrix)
    {
        depthMapShader.useShaderProgram();
        depthMapShader.setMat4("lightSpaceTrMatrix", glm::value_ptr(lightSpaceTrMatrix));
        shader.setFloat("heightScale", heightScale);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightMapTexture);
        shader.setInt("heightMap", 1);

        glBindVertexArray(model.getVAO());
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glVertexAttribDivisor(3, 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, model.getVertexCount(), instancePositions.size());
        glBindVertexArray(0);
    }

    std::vector<glm::vec3> Tree::generateTreePositions()
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
                glm::vec3 position(x * spacing + offsetX, 0.0f, z * spacing + offsetZ);

                if (glm::length(glm::vec2(position.x, position.z)) <= radius && glm::length(glm::vec2(position.x, position.z)) > 10.0f)
                {
                    positions.emplace_back(position);
                }


            }
        }
        return positions;
    }

}