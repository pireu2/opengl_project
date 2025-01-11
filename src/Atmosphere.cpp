#include <Atmosphere.hpp>

namespace gps
{

    void Atmosphere::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        shader.loadShader(vertexShaderFileName, fragmentShaderFileName);
        shader.useShaderProgram();
    }

    void Atmosphere::setUniforms(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition, float time)
    {
        shader.useShaderProgram();
        shader.setFloat("fogDensity", fogDensity);
        shader.setVec3("fogColor", fogColor);
        shader.setFloat("fogOffset", fogOffset);
        shader.setVec3("sunColor", sunColor);
        shader.setVec3("sunDirection", sunDirection);
        shader.setFloat("fogHeight", fogHeight);
        shader.setFloat("fogAttenuation", fogAttenuation);
        shader.setFloat("skyboxSpeed", skyboxSpeed);
        shader.setVec3("skyboxDirection", skyboxDirection);
        shader.setMat4("cameraInvViewProjection", glm::value_ptr(glm::inverse(projection * view)));
        shader.setVec3("cameraPos", glm::value_ptr(cameraPosition));
        shader.setFloat("distanceFog", distanceFog);
        shader.setFloat("time", time);
    }

    void Atmosphere::setSceneTexture(int textureId)
    {
        shader.useShaderProgram();
        shader.setInt("scene", textureId);
    }

    void Atmosphere::setDepthTexture(int textureId)
    {
        shader.useShaderProgram();
        shader.setInt("depthTexture", textureId);
    }

    void Atmosphere::setSkyboxTexture(int textureId)
    {
        shader.useShaderProgram();
        shader.setInt("skyboxTex", textureId);
    }

    void Atmosphere::drawImguiControls()
    {
        ImGui::Begin("Atmosphere Parameters");
        ImGui::SliderFloat("Fog Density", &fogDensity, 0.0f, 10.0f);
        ImGui::ColorEdit3("Fog Color", (float *)&fogColor);
        ImGui::SliderFloat("Fog Offset", &fogOffset, 0.0f, 4000.0f);
        ImGui::SliderFloat("Fog Height", &fogHeight, 0.0f, 1000.0f);
        ImGui::SliderFloat("Fog Attenuation", &fogAttenuation, 0.0f, 1.0f);
        ImGui::SliderFloat("Distance Fog", &distanceFog, 0.0f, 4000.0f);
        ImGui::ColorEdit3("Sun Color", (float *)&sunColor);
        ImGui::SliderFloat("Sun Direction X", &sunDirection.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Sun Direction Y", &sunDirection.y, -1.0f, 1.0f);
        ImGui::SliderFloat("Sun Direction Z", &sunDirection.z, -1.0f, 1.0f);
        ImGui::SliderFloat("Skybox Speed", &skyboxSpeed, 0.0f, 1.0f);
        ImGui::SliderFloat("Skybox Direction X", &skyboxDirection.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Skybox Direction Y", &skyboxDirection.y, -1.0f, 1.0f);
        ImGui::SliderFloat("Skybox Direction Z", &skyboxDirection.z, -1.0f, 1.0f);

        ImGui::End();
    }
}