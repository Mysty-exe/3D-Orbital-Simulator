#include <Rendering/Model.h>
#include "Rendering/stb_image.h"

Model::Model()
{
}

Model::Model(const std::string &path)
{
    loadModel(path);
}

void Model::draw(Shader &shader, bool depth)
{
    shader.use();
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        if (!depth)
            meshes[i].draw(shader);
        else
            meshes[i].depthDraw(shader);
    }
}

std::vector<ObjectMesh> Model::getMeshes()
{
    return meshes;
}

void Model::cleanup()
{
    for (int i = 0; i < meshes.size(); i++)
        meshes[i].cleanup();
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    modelRadius = computeBaseRadius();
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ObjectMesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;
    glm::vec3 vector;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        vertex.Position = vector;
        vertex.Normal = glm::vec3(0.0f);

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material,
                                                                                 aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material,
                                                                                  aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<std::shared_ptr<Texture>> emissiveMaps = loadMaterialTextures(material,
                                                                                  aiTextureType_EMISSIVE);
        textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
    }

    return ObjectMesh(vertices, indices, textures);
}

float Model::computeBaseRadius()
{
    float maxDist = 0.0f;

    for (auto &mesh : meshes)
    {
        for (const Vertex &v : mesh.getVertices())
        {
            float dist = glm::length(v.Position);
            if (dist > maxDist)
                maxDist = dist;
        }
    }

    return maxDist;
}

TextureType Model::convertType(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE:
        return TextureType::Diffuse;
    case aiTextureType_SPECULAR:
        return TextureType::Specular;
    case aiTextureType_EMISSIVE:
        return TextureType::Emissive;
    default:
        return TextureType::Unknown;
    }
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j]->path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>();
            std::string filename;
            std::string texPath = str.C_Str();

            if (texPath.find(':') != std::string::npos)
                texPath = texPath.substr(3);

            filename = directory + "/" + texPath;

            texture->path = str.C_Str();
            texture->type = convertType(type);

            texture->data = stbi_load(filename.c_str(),
                                      &texture->width,
                                      &texture->height,
                                      &texture->channels,
                                      0);

            if (!texture->data)
            {
                std::cout << "Failed to load texture: " << filename << std::endl;
            }

            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

void Model::upload()
{
    for (auto &mesh : meshes)
    {
        mesh.upload();
    }
}