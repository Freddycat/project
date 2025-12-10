#include "meshes.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model 
{
    public:
        Model(char *path)
        {
            LoadModel(path);
        }
        void Draw(GLuint &shader);	
    private:
        // model data
        vector<Mesh> meshes;
        string directory;

        void LoadModel(string path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
        Material GetMeshMaterial(const aiMaterial *mtl);
        vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};