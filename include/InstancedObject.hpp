#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <Shader.hpp>
#include <Model3D.hpp>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace gps
{

    class InstancedObject
    {
    public:
        void loadModel(const std::string &modelPath);
        void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
        void setInstancePositions(const std::vector<glm::vec3> &positions);
        virtual void drawImguiControls() = 0;

    protected:
        Shader shader = {};
        Shader depthMapShader = {};
        Model3D model;
        unsigned int instanceVBO = 0;
        std::vector<glm::vec3> instancePositions;
    };

}