#pragma once
#include <Rendering/Shader.h>
#include <vector>
#include <glad/glad.h>
#include "Rendering/stb_image.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

enum class TextureType
{
    Diffuse,
    Specular,
    Emissive,
    Unknown
};

struct Material
{
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Ambient;
    float Shininess;
};

struct Texture
{
    unsigned int id = 0;

    TextureType type;
    std::string path;

    int width, height, channels;
    unsigned char *data = nullptr;

    bool uploaded = false;

    void upload()
    {
        if (uploaded || !data || width <= 0 || height <= 0)
            return;

        GLenum format = GL_RGB;

        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else
            return;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        data = nullptr;

        uploaded = true;
    }
};

class Mesh
{
protected:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

public:
    Mesh();
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<Texture>> &textures = {});
    virtual void setUpMesh();
    virtual void draw(Shader &shader) const;
    void cleanup() const;
    std::vector<Vertex> getVertices();
    std::vector<unsigned int> getIndices();
    std::vector<std::shared_ptr<Texture>> getTextures();
    unsigned int getVAO();
};
