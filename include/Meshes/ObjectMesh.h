#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Meshes/Mesh.h"
#include "Utils/Vector.h"

class ObjectMesh : public Mesh
{
public:
    ObjectMesh();
    ObjectMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<std::shared_ptr<Texture>> &textures);
    void setUpMesh();
    void upload();
    void draw(Shader &shader);
    void depthDraw(Shader &shader);
};
