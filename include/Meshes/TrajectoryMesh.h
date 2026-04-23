#include <iostream>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Meshes/Mesh.h"

class TrajectoryMesh : public Mesh
{
public:
    TrajectoryMesh();
    void setVertices(std::vector<Vertex> &vertices);
    void setUpMesh() override;
    void draw(Shader &shader) const override;
};