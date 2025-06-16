#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



unsigned int TextureFromFile(const char *path, const std::string &directory);


class Model{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model() = default;
    Model(char* path, bool gamma = false) : gammaCorrection(gamma){
        loadModel(path);
    }
    void init(char *path, bool gamma = false);
    void Draw(Shader &shader, Camera& camera);
    
private:

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    const std::string m_basePath = std::filesystem::current_path().string();
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};