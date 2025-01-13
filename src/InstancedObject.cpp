#include <InstancedObject.hpp>

namespace gps
{

    void InstancedObject::loadModel(const std::string &modelPath)
    {
        model.LoadModel(modelPath);
    }

    void InstancedObject::setShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName)
    {
        shader.loadShader(vertexShaderFileName, fragmentShaderFileName);
    }

    void InstancedObject::setInstancePositions(const std::vector<glm::vec3> &positions)
    {
        this->instancePositions = positions;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instancePositions.size() * sizeof(glm::vec3), &instancePositions[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}