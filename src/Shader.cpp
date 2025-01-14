#include "Shader.hpp"

namespace gps
{
    std::string Shader::readShaderFile(const std::string &fileName)
    {
        std::ifstream shaderFile;

        shaderFile.open(fileName);

        std::stringstream shaderStringStream;

        shaderStringStream << shaderFile.rdbuf();

        shaderFile.close();

        std::string shaderString = shaderStringStream.str();
        return shaderString;
    }

    void Shader::shaderCompileLog(const int shaderId)
    {
        int success;

        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
            std::cout << "Shader compilation error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::shaderLinkLog(const int shaderProgramId) const
    {
        int success;

        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "Shader linking error\n"
                      << infoLog << std::endl;
        }
    }

    void Shader::validateShaderProgram(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName) const
    {
        glValidateProgram(this->shaderProgram);
        int success;
        glGetProgramiv(this->shaderProgram, GL_VALIDATE_STATUS, &success);
        if (!success)
        {
            int logSize;
            glGetProgramiv(this->shaderProgram, GL_INFO_LOG_LENGTH, &logSize);
            if (logSize > 0)
            {
                std::vector<char> log(logSize);
                glGetProgramInfoLog(this->shaderProgram, logSize, &logSize, log.data());
                std::cerr << "Shader program validation error in files: " << vertexShaderFileName << " and " << fragmentShaderFileName << "\n" << log.data() << std::endl;
            }
            else
            {
                std::cerr << "Shader program validation error in files: " << vertexShaderFileName << " and " << fragmentShaderFileName << ": No additional information available." << std::endl;
            }
        }
    }

    void Shader::checkOpenGLErrors()
    {
        if (const GLenum error = glGetError(); error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error: " << error << std::endl;
        }
    }

    void Shader::loadShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        std::string v = readShaderFile(vertexShaderFileName);
        const GLchar *vertexShaderString = v.c_str();
        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderString, nullptr);
        glCompileShader(vertexShader);
        shaderCompileLog(vertexShader);

        std::string f = readShaderFile(fragmentShaderFileName);
        const GLchar *fragmentShaderString = f.c_str();
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderString, nullptr);
        glCompileShader(fragmentShader);
        shaderCompileLog(fragmentShader);

        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertexShader);
        glAttachShader(this->shaderProgram, fragmentShader);
        glLinkProgram(this->shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        shaderLinkLog(this->shaderProgram);

        validateShaderProgram(vertexShaderFileName, fragmentShaderFileName);

        checkOpenGLErrors();
    }

    void Shader::useShaderProgram() const
    {
        glUseProgram(this->shaderProgram);
    }

    void Shader::setMat4(const std::string &name, glm::mat4 &value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    void Shader::setMat3(const std::string &name, glm::mat3 &value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    void Shader::setVec3(const std::string &name, glm::vec3 &value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform3fv(location, 1, glm::value_ptr(value));
        }
    }

    void Shader::setVec4(const std::string &name, glm::vec4 &value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform4fv(location, 1, glm::value_ptr(value));
        }
    }

    void Shader::setMat4(const std::string &name, const float *value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniformMatrix4fv(location, 1, GL_FALSE, value);
        }
    }

    void Shader::setMat3(const std::string &name, const float *value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniformMatrix3fv(location, 1, GL_FALSE, value);
        }
    }

    void Shader::setVec3(const std::string &name, const float *value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform3f(location, value[0], value[1], value[2]);
        }
    }

    void Shader::setVec4(const std::string &name, const float *value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform4f(location, value[0], value[1], value[2], value[3]);
        }
    }

    void Shader::setInt(const std::string &name, const int value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform1i(location, value);
        }
    }

    void Shader::setFloat(const std::string &name, const float value) const
    {
        if (const int location = glGetUniformLocation(shaderProgram, name.c_str()); location == -1) {
            std::cerr << "Error: Could not find uniform location for '" << name << "'" << std::endl;
        } else {
            glUniform1f(location, value);
        }
    }

}
