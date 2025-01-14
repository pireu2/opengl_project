#include <Water.hpp>

namespace gps
{

    void Water::initUniforms(const glm::mat4 &modelMatrix, const glm::mat4 &view, const glm::mat3 &normalMatrix, const glm::vec3 &lightDir,
                             const glm::vec3 &lightColor, const glm::vec3 &viewPos)
    {
        shader.useShaderProgram();

        shader.setMat4("model", value_ptr(modelMatrix));
        shader.setMat4("view", value_ptr(view));

        shader.setVec3("lightDir", value_ptr(lightDir));
        shader.setVec3("viewPos", value_ptr(viewPos));
        shader.setVec3("lightColor", value_ptr(lightColor));

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
        shader.setFloat("time", static_cast<float>(glfwGetTime()));

        shader.setFloat("ambientStrength", ambientStrength);
        shader.setVec3("ambientColor", ambientColor);
        shader.setVec3("diffuseColor", diffuseColor);
        shader.setVec3("specularColor", specularColor);
        shader.setFloat("shininess", shininess);

        shader.setFloat("tipAttenuation", tipAttenuation);
        shader.setVec3("tipColor", tipColor);
        shader.setFloat("mixStrength", mixStrength);

        shader.setInt("skyboxTex", 2);
        shader.setFloat("fresnelNormalStrength", fresnelNormalStrength);
        shader.setFloat("fresnelShininess", fresnelShininess);
        shader.setFloat("fresnelBias", fresnelBias);
        shader.setFloat("fresnelStrength", fresnelStrength);
    }

    void Water::render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection,
                       const glm::mat3 &normalMatrix, const glm::vec3 &lightDir, const glm::vec3 &lightColor, const glm::vec3 &viewPos)
    {
        shader.useShaderProgram();
        shader.setMat4("model", value_ptr(model));
        shader.setMat4("view", value_ptr(view));
        shader.setMat4("projection", value_ptr(projection));
        shader.setVec3("lightDir", value_ptr(lightDir));
        shader.setVec3("lightColor", value_ptr(lightColor));

        // shader.setMat3("normalMatrix", normalMatrix);
        shader.setVec3("viewPos", value_ptr(viewPos));
        shader.setFloat("time", static_cast<float>(glfwGetTime()));

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

        shader.setFloat("ambientStrength", ambientStrength);
        shader.setVec3("ambientColor", ambientColor);
        shader.setVec3("diffuseColor", diffuseColor);
        shader.setVec3("specularColor", specularColor);
        shader.setFloat("shininess", shininess);
        shader.setFloat("tipAttenuation", tipAttenuation);
        shader.setVec3("tipColor", tipColor);
        shader.setFloat("mixStrength", mixStrength);

        shader.setFloat("fresnelNormalStrength", fresnelNormalStrength);
        shader.setFloat("fresnelShininess", fresnelShininess);
        shader.setFloat("fresnelBias", fresnelBias);
        shader.setFloat("fresnelStrength", fresnelStrength);
        shader.setInt("skyboxTex", 2);

        this->model.Draw(shader);
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
        ImGui::SliderFloat("Mix Strength", &mixStrength, 0.0f, 1.0f);
        ImGui::End();

        ImGui::Begin("Fresnel Properties");
        ImGui::SliderFloat("Fresnel Normal Strength", &fresnelNormalStrength, 0.0f, 1.0f);
        ImGui::SliderFloat("Fresnel Shininess", &fresnelShininess, 0.0f, 10.0f);
        ImGui::SliderFloat("Fresnel Bias", &fresnelBias, 0.0f, 1.0f);
        ImGui::SliderFloat("Fresnel Strength", &fresnelStrength, 0.0f, 1.0f);
        ImGui::End();
    }

    void Water::loadModel(const std::string &fileName)
    {
        model.LoadModel(fileName);
    }

    void Water::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        shader.loadShader(vertexShaderFileName, fragmentShaderFileName);
        shader.useShaderProgram();
    }

}