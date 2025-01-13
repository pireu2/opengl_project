#include <InstancedObject.hpp>

namespace gps {

    void InstancedObject::loadModel(const std::string &modelPath) {
        model.LoadModel(modelPath);
    }

    void InstancedObject::setShader(const std::string &vertexShaderFileName, const std::string &fragmentShaderFileName) {
        shader.loadShader(vertexShaderFileName, fragmentShaderFileName);
    }
}