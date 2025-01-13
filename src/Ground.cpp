#include <Ground.hpp>

namespace gps
{
    void Ground::init()
    {
        heightMapTexture = Model3D::ReadTextureFromFile(RESOURCES_PATH "textures/heightmap.png");
    }

    void Ground::loadModel(const std::string& path)
    {
        model.LoadModel(path);
    }

    void Ground::loadShader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        shader.loadShader(vertexPath, fragmentPath);
        shader.useShaderProgram();
    }

    void Ground::initUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::mat3& normalMatrix, const glm::vec3& lightDir, const glm::vec3& lightColor)
    {
        shader.useShaderProgram();
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
        shader.setVec3("lightDir", glm::value_ptr(lightDir));
        shader.setVec3("lightColor", glm::value_ptr(lightColor));

    }

    void Ground::render(const glm::mat4& view, const glm::mat4& projection, const glm::mat3& normalMatrix, const glm::vec3& lightDir)
    {
        shader.useShaderProgram();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat3("normalMatrix", glm::value_ptr(normalMatrix));
        shader.setVec3("lightDir", glm::value_ptr(lightDir));
        shader.setFloat("heightScale", heightScale);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightMapTexture);
        shader.setInt("heightMap", 1);
        model.Draw(shader);
    }
}