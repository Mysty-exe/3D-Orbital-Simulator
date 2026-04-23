#include <Meshes/TrajectoryMesh.h>

TrajectoryMesh::TrajectoryMesh() : Mesh(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<std::shared_ptr<Texture>>())
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    setUpMesh();
}

void TrajectoryMesh::setVertices(std::vector<Vertex> &vertices)
{
    this->vertices = vertices;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TrajectoryMesh::setUpMesh()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void TrajectoryMesh::draw(Shader &shader) const
{
    shader.use();

    glBindVertexArray(VAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);
}
