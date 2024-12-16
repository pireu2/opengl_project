#include <Water.hpp>

namespace gps
{

    void Water::initUniforms(glm::mat4 modelMatrix, glm::mat4 view, glm::mat3 normalMatrix, glm::vec3 lightPos, glm::vec3 lightColor, glm::vec3 viewPos)
    {
        shader.useShaderProgram();

        shader.setMat4("model", modelMatrix);
        shader.setMat4("view", view);
        shader.setMat3("normalMatrix", normalMatrix);

        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", viewPos);
        shader.setVec3("lightColor", lightColor);

        shader.setInt("vertexWaveCount", vertexWaveCount);
        shader.setFloat("vertexSeed", vertexSeed);
        shader.setFloat("vertexSeedIter", vertexSeedIter);
        shader.setFloat("vertexFrequency", vertexFrequency);
        shader.setFloat("vertexFrequencyMult", vertexFrequencyMult);
        shader.setFloat("vertexAmplitude", vertexAmplitude);
        shader.setFloat("vertexAmplitudeMult", vertexAmplitudeMult);
        shader.setFloat("vertexInitialSpeed", vertexInitialSpeed);
        shader.setFloat("vertexSpeedRamp", vertexSpeedRamp);
        shader.setFloat("vertexDrag", vertexDrag);
        shader.setFloat("vertexHeight", vertexHeight);
        shader.setFloat("vertexMaxPeak", vertexMaxPeak);
        shader.setFloat("vertexPeakOffset", vertexPeakOffset);

        shader.setFloat("fragmentSeed", fragmentSeed);
        shader.setFloat("fragmentSeedIter", fragmentSeedIter);
        shader.setFloat("fragmentFrequency", fragmentFrequency);
        shader.setFloat("fragmentFrequencyMult", fragmentFrequencyMult);
        shader.setFloat("fragmentAmplitude", fragmentAmplitude);
        shader.setFloat("fragmentAmplitudeMult", fragmentAmplitudeMult);
        shader.setFloat("fragmentInitialSpeed", fragmentInitialSpeed);
        shader.setFloat("fragmentSpeedRamp", fragmentSpeedRamp);
        shader.setFloat("fragmentDrag", fragmentDrag);
        shader.setFloat("fragmentHeight", fragmentHeight);
        shader.setFloat("fragmentMaxPeak", fragmentMaxPeak);
        shader.setFloat("fragmentPeakOffset", fragmentPeakOffset);
        shader.setInt("fragmentWaveCount", fragmentWaveCount);
        shader.setFloat("time", timeWater);

        shader.setFloat("ambientStrength", ambientStrength);
        shader.setVec3("ambientColor", ambientColor);
        shader.setVec3("diffuseColor", diffuseColor);
        shader.setVec3("specularColor", specularColor);
        shader.setFloat("shininess", shininess);

        shader.setFloat("tipAttenuation", tipAttenuation);
        shader.setVec3("tipColor", tipColor);
    }

    void Water::setUniforms(glm::mat4 view, glm::mat4 projection, glm::vec3 lightPos, glm::vec3 viewPos)
    {
        shader.useShaderProgram();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", viewPos);
        shader.setFloat("time", timeWater);

        shader.setInt("vertexWaveCount", vertexWaveCount);
        shader.setFloat("vertexSeed", vertexSeed);
        shader.setFloat("vertexSeedIter", vertexSeedIter);
        shader.setFloat("vertexFrequency", vertexFrequency);
        shader.setFloat("vertexFrequencyMult", vertexFrequencyMult);
        shader.setFloat("vertexAmplitude", vertexAmplitude);
        shader.setFloat("vertexAmplitudeMult", vertexAmplitudeMult);
        shader.setFloat("vertexInitialSpeed", vertexInitialSpeed);
        shader.setFloat("vertexSpeedRamp", vertexSpeedRamp);
        shader.setFloat("vertexDrag", vertexDrag);
        shader.setFloat("vertexHeight", vertexHeight);
        shader.setFloat("vertexMaxPeak", vertexMaxPeak);
        shader.setFloat("vertexPeakOffset", vertexPeakOffset);

        shader.setFloat("fragmentSeed", fragmentSeed);
        shader.setFloat("fragmentSeedIter", fragmentSeedIter);
        shader.setFloat("fragmentFrequency", fragmentFrequency);
        shader.setFloat("fragmentFrequencyMult", fragmentFrequencyMult);
        shader.setFloat("fragmentAmplitude", fragmentAmplitude);
        shader.setFloat("fragmentAmplitudeMult", fragmentAmplitudeMult);
        shader.setFloat("fragmentInitialSpeed", fragmentInitialSpeed);
        shader.setFloat("fragmentSpeedRamp", fragmentSpeedRamp);
        shader.setFloat("fragmentDrag", fragmentDrag);
        shader.setFloat("fragmentHeight", fragmentHeight);
        shader.setFloat("fragmentMaxPeak", fragmentMaxPeak);
        shader.setFloat("fragmentPeakOffset", fragmentPeakOffset);
        shader.setInt("fragmentWaveCount", fragmentWaveCount);
        shader.setFloat("time", timeWater);

        shader.setFloat("ambientStrength", ambientStrength);
        shader.setVec3("ambientColor", ambientColor);
        shader.setVec3("diffuseColor", diffuseColor);
        shader.setVec3("specularColor", specularColor);
        shader.setFloat("shininess", shininess);
        shader.setFloat("tipAttenuation", tipAttenuation);
        shader.setVec3("tipColor", tipColor);
    }

    void Water::drawImguiControls()
    {
        ImGui::Begin("Water Parameters");
        ImGui::SliderInt("Vertex Wave Count", &vertexWaveCount, 1, 20);
        ImGui::SliderFloat("Vertex Seed", &vertexSeed, 0.0f, 1000.0f);
        ImGui::SliderFloat("Vertex Seed Iter", &vertexSeedIter, 0.0f, 1000.0f);
        ImGui::SliderFloat("Vertex Frequency", &vertexFrequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Vertex Frequency Mult", &vertexFrequencyMult, 0.1f, 10.0f);
        ImGui::SliderFloat("Vertex Amplitude", &vertexAmplitude, 0.0f, 1.0f);
        ImGui::SliderFloat("Vertex Amplitude Mult", &vertexAmplitudeMult, 0.0f, 1.0f);
        ImGui::SliderFloat("Vertex Initial Speed", &vertexInitialSpeed, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Speed Ramp", &vertexSpeedRamp, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Drag", &vertexDrag, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Height", &vertexHeight, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Max Peak", &vertexMaxPeak, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Peak Offset", &vertexPeakOffset, 0.0f, 10.0f);
        ImGui::SliderInt("Fragment Wave Count", &fragmentWaveCount, 1, 50);
        ImGui::SliderFloat("Fragment Seed", &fragmentSeed, 0.0f, 1000.0f);
        ImGui::SliderFloat("Fragment Seed Iter", &fragmentSeedIter, 0.0f, 1000.0f);
        ImGui::SliderFloat("Fragment Frequency", &fragmentFrequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Fragment Frequency Mult", &fragmentFrequencyMult, 0.1f, 10.0f);
        ImGui::SliderFloat("Fragment Amplitude", &fragmentAmplitude, 0.0f, 1.0f);
        ImGui::SliderFloat("Fragment Amplitude Mult", &fragmentAmplitudeMult, 0.0f, 1.0f);
        ImGui::SliderFloat("Fragment Initial Speed", &fragmentInitialSpeed, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Speed Ramp", &fragmentSpeedRamp, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Drag", &fragmentDrag, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Height", &fragmentHeight, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Max Peak", &fragmentMaxPeak, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Peak Offset", &fragmentPeakOffset, 0.0f, 10.0f);
        ImGui::End();

        ImGui::Begin("Material Properties");
        ImGui::ColorEdit3("Ambient Color", (float *)&ambientColor);
        ImGui::ColorEdit3("Diffuse Color", (float *)&diffuseColor);
        ImGui::ColorEdit3("Specular Color", (float *)&specularColor);
        ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
        ImGui::ColorEdit3("Tip Color", (float *)&tipColor);
        ImGui::SliderFloat("Tip Attenuation", &tipAttenuation, 0.0f, 10.0f);
        ImGui::End();
    }

    void Water::loadModel(std::string fileName)
    {
        model.LoadModel(fileName);
    }

    void Water::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        shader.loadShader(vertexShaderFileName, fragmentShaderFileName);
        shader.useShaderProgram();
    }

    void Water::draw(glm::mat4 view)
    {
        shader.useShaderProgram();

        auto modelMatrix = glm::mat4(1.0f);
        shader.setMat4("model", modelMatrix);

        auto normalMatrix = glm::mat3(glm::inverseTranspose(view * modelMatrix));
        shader.setMat3("normalMatrix", normalMatrix);

        model.Draw(shader);
    }
}