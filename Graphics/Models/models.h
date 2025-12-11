#ifndef MODELS_H
#define MODELS_H

#include "shapes.h"
#include "meshes.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <string>

class Model {
  public:
    Model() = default;
    Model(std::string path)
    {
        LoadModel(path);
    }
    void Draw(GLuint &shader, Transform & transform);
    
    vector<Mesh> meshes;
  private:
    // model data
    string directory;

    void LoadModel(string path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    Material GetMeshMaterial(const aiMaterial *mtl);
    vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);
};

#endif