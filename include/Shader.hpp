
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

namespace gps
{

    class Shader
    {

    public:
        GLuint shaderProgram;
        void loadShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
        void useShaderProgram();

        void setMat4(const std::string &name, glm::mat4 &value);
        void setMat3(const std::string &name, glm::mat3 &value);
        void setVec3(const std::string &name, glm::vec3 &value);
        void setVec4(const std::string &name, glm::vec4 &value);
        void setMat4(const std::string &name, const float *value);
        void setMat3(const std::string &name, const float *value);
        void setVec3(const std::string &name, const float *value);
        void setVec4(const std::string &name, const float *value);
        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);

    private:
        std::string readShaderFile(std::string fileName);
        void shaderCompileLog(GLuint shaderId);
        void shaderLinkLog(GLuint shaderProgramId);
    };

}
