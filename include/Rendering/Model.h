#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Meshes/ObjectMesh.h>
#include <Rendering/Shader.h>
#include "Rendering/stb_image.h"

class Model
{
private:
    float modelRadius;
    std::string directory;
    std::vector<std::shared_ptr<Texture>> textures_loaded;
    std::vector<ObjectMesh> meshes;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    ObjectMesh processMesh(aiMesh *mesh, const aiScene *scene);
    float computeBaseRadius();
    TextureType convertType(aiTextureType type);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

public:
    Model();
    Model(const std::string &path);
    std::string getPath() { return directory; }
    void draw(Shader &shader, bool depth = false);
    std::vector<ObjectMesh> getMeshes();
    float getModelRadius() { return modelRadius; }
    void upload();
    void cleanup();
};
