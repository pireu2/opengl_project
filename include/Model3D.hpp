#pragma once

#include "Mesh.hpp"

#include <stb_image/stb_image.h>
#include <tiny_obj_loader/tiny_obj_loader.h>

#include <iostream>
#include <string>
#include <vector>

namespace gps
{

    class Model3D
    {

    public:
        ~Model3D();

        void LoadModel(std::string fileName);

        void LoadModel(std::string fileName, std::string basePath);

        void Draw(Shader shaderProgram);

        static GLuint ReadTextureFromFile(const char *file_name);
    private:
        // Component meshes - group of objects
        std::vector<Mesh> meshes;
        // Associated textures
        std::vector<Texture> loadedTextures;

        // Does the parsing of the .obj file and fills in the data structure
        void ReadOBJ(std::string fileName, std::string basePath);

        // Retrieves a texture associated with the object - by its name and type
        Texture LoadTexture(std::string path, std::string type);

        // Reads the pixel data from an image file and loads it into the video memory

    };
}
