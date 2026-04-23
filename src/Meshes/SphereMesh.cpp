#include "Meshes/SphereMesh.h"

SphereMesh::SphereMesh() : Mesh(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<std::shared_ptr<Texture>>())
{
}

SphereMesh::SphereMesh(int stacks, int slices, float radius) : Mesh(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<std::shared_ptr<Texture>>())
{
    for (int i = 0; i <= stacks; ++i)
    {
        float phi = glm::pi<float>() * i / stacks;
        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * glm::pi<float>() * j / slices;

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            Vertex pos;
            pos.Position = glm::vec3(x, y, z);
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            pos.Normal = normal;

            vertices.push_back(pos);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int p1 = i * (slices + 1) + j;
            int p2 = p1 + (slices + 1);
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }

    setUpMesh();
}

void SphereMesh::setUpMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}
