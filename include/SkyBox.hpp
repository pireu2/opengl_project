#pragma once

#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>

#include <vector>

namespace gps
{
    class SkyBox
    {
    public:
        SkyBox();
        void Load(const std::vector<const GLchar *> &cubeMapFaces);
        void Draw(Shader shader, const glm::mat4 &viewMatrix, glm::mat4 projectionMatrix) const;
        void LoadFromDir(const std::string &filePath);
        [[nodiscard]] GLuint GetTextureId() const;

    private:
        GLuint skyboxVAO = 0;
        GLuint skyboxVBO = 0;
        GLuint cubemapTexture = 0;
        static GLuint LoadSkyBoxTextures(const std::vector<const GLchar *> &cubeMapFaces);
        void InitSkyBox();
    };
}
