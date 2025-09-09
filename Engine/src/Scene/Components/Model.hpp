#pragma once
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Components/Camera.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// class Component {
// public:
//     Scene* Scene = nullptr;

//     int ID = 0;
    
//     glm::vec3 Position = {0, 0, 0};
//     glm::vec3 Rotation = {0, 0, 0};
//     glm::vec3 Scale = {1, 1, 1};

//     std::vector<std::unique_ptr<Component>> childComponents;
//     std::unordered_map<int, Component*> childLookup;


class Model : public Component{
public:
    std::vector<Texture> TexturesLoaded;
    std::vector<Mesh> Meshes;
    std::string Directory;
    bool GammaCorrection;

    Model() = default;

    Model(const char* path, bool gamma = false) : GammaCorrection(gamma){
        loadModel(path);
    }

    void Draw(Shader &shader, Camera& camera);

    void DrawMask(Shader &shader, Camera& camera, std::vector<GLuint> ID);

    static unsigned int TextureFromFile(const char *path, const std::string directory, int* width = nullptr, int* height = nullptr);

    static glm::mat4 CreateModelMatrix(const glm::vec3& position,
                            const glm::vec3& rotationDeg,
                            const glm::vec3& scale);

    void loadModel(std::string path);
private:
    const std::string m_BasePath = std::filesystem::current_path().string();

    
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                            aiTextureType type,
                                            std::string typeName);
    
    

};