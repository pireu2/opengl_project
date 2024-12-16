
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <fstream>
#include <iostream>

namespace gps
{

    class Shader
    {

    public:
        GLuint shaderProgram;
        void loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName);
        void useShaderProgram() const;

        void setMat4(const std::string &name, glm::mat4 &value) const;
        void setMat3(const std::string &name, glm::mat3 &value) const;
        void setVec3(const std::string &name, glm::vec3 &value) const;
        void setVec4(const std::string &name, glm::vec4 &value) const;
        void setMat4(const std::string &name, const float *value) const;
        void setMat3(const std::string &name, const float *value) const;
        void setVec3(const std::string &name, const float *value) const;
        void setVec4(const std::string &name, const float *value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

    private:
        static std::string readShaderFile(const std::string& fileName);
        static void shaderCompileLog(GLuint shaderId);
        void shaderLinkLog(GLuint shaderProgramId) const;
    };

}
