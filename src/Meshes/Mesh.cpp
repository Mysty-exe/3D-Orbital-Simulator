#include "Meshes/Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<Texture>> &textures) : vertices(vertices), indices(indices), textures(textures)
{
}

void Mesh::setUpMesh()
{
}

void Mesh::draw(Shader &shader) const
{
    shader.use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::cleanup() const
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

std::vector<Vertex> Mesh::getVertices()
{
    return vertices;
}

std::vector<unsigned int> Mesh::getIndices()
{
    return indices;
}

std::vector<std::shared_ptr<Texture>> Mesh::getTextures()
{
    return textures;
}

unsigned int Mesh::getVAO()
{
    return VAO;
}
