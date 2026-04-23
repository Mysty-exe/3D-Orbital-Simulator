#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Meshes/Mesh.h"
#include "Utils/Vector.h"

class SphereMesh : public Mesh
{
public:
    SphereMesh();
    SphereMesh(int stacks, int slices, float radius);
    void setUpMesh();
};