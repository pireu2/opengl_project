//
//  Shader.cpp
//  Lab3
//
//  Created by CGIS on 05/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "Shader.hpp"

namespace gps
{
    std::string Shader::readShaderFile(const std::string& fileName)
    {

        std::ifstream shaderFile;

        // open shader file
        shaderFile.open(fileName);

        std::stringstream shaderStringStream;

        // read shader content into stream
        shaderStringStream << shaderFile.rdbuf();

        // close shader file
        shaderFile.close();

        // convert stream into GLchar array
        std::string shaderString = shaderStringStream.str();
        return shaderString;
    }

    void Shader::shaderCompileLog(GLuint shaderId)
    {

        GLint success;

        // check compilation info
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLchar infoLog[512];

            glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
            std::cout << "Shader compilation error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::shaderLinkLog(GLuint shaderProgramId) const
    {

        GLint success;

        // check linking info
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "Shader linking error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {

        // read, parse and compile the vertex shader
        std::string v = readShaderFile(vertexShaderFileName);
        const GLchar *vertexShaderString = v.c_str();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderString, nullptr);
        glCompileShader(vertexShader);
        // check compilation status
        shaderCompileLog(vertexShader);

        // read, parse and compile the vertex shader
        std::string f = readShaderFile(fragmentShaderFileName);
        const GLchar *fragmentShaderString = f.c_str();
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderString, nullptr);
        glCompileShader(fragmentShader);
        // check compilation status
        shaderCompileLog(fragmentShader);

        // attach and link the shader programs
        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertexShader);
        glAttachShader(this->shaderProgram, fragmentShader);
        glLinkProgram(this->shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // check linking info
        shaderLinkLog(this->shaderProgram);
    }

    void Shader::useShaderProgram() const
    {

        glUseProgram(this->shaderProgram);
    }

    void Shader::setMat4(const std::string &name, glm::mat4 &value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::setMat3(const std::string &name, glm::mat3 &value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::setVec3(const std::string &name, glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value_ptr(value));
    }

    void Shader::setVec4(const std::string &name, glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, value_ptr(value));
    }

    void Shader::setMat4(const std::string &name, const float *value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value);
    }

    void Shader::setMat3(const std::string &name, const float *value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, value);
    }

    void Shader::setVec3(const std::string &name, const float *value) const
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2]);
    }

    void Shader::setVec4(const std::string &name, const float *value) const
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2], value[3]);
    }

    void Shader::setInt(const std::string &name, const int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, const float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

}
