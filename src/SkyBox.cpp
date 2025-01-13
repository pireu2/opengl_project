#include "SkyBox.hpp"

namespace gps
{

    SkyBox::SkyBox() = default;

    void SkyBox::Load(const std::vector<const GLchar *> &cubeMapFaces)
    {
        cubemapTexture = LoadSkyBoxTextures(cubeMapFaces);
        InitSkyBox();
    }

    void SkyBox::Draw(const Shader shader, const glm::mat4 &viewMatrix, glm::mat4 projectionMatrix) const
    {
        shader.useShaderProgram();

        // set the view and projection matrices
        glm::mat4 transformedView = glm::mat4(glm::mat3(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, value_ptr(transformedView));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, value_ptr(projectionMatrix));

        glDepthFunc(GL_LEQUAL);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader.shaderProgram, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
    }

    void SkyBox::LoadFromDir(const std::string &filePath)
    {
        const std::vector facePaths = {
            filePath + "px.png", // positive x
            filePath + "nx.png", // negative x
            filePath + "py.png", // positive y
            filePath + "ny.png", // negative y
            filePath + "pz.png", // positive z
            filePath + "nz.png"  // negative z
        };

        std::vector<const GLchar *> faces;
        faces.reserve(facePaths.size());
        for (const auto &facePath : facePaths)
        {
            faces.push_back(facePath.c_str());
        }

        this->Load(faces);
    }

    GLuint SkyBox::LoadSkyBoxTextures(const std::vector<const GLchar *> &cubeMapFaces)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);

        int width, height, n;

        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for (GLuint i = 0; i < cubeMapFaces.size(); i++)
        {
            constexpr int force_channels = 3;
            const unsigned char *image = stbi_load(cubeMapFaces[i], &width, &height, &n, force_channels);
            if (!image)
            {
                fprintf(stderr, "ERROR: could not load %s\n", cubeMapFaces[i]);
                return false;
            }
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return textureID;
    }

    void SkyBox::InitSkyBox()
    {
        constexpr GLfloat skyboxVertices[] = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f};

        glGenVertexArrays(1, &this->skyboxVAO);
        glGenBuffers(1, &skyboxVBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid *>(nullptr));

        glBindVertexArray(0);
    }

    GLuint SkyBox::GetTextureId() const
    {
        return cubemapTexture;
    }
}
