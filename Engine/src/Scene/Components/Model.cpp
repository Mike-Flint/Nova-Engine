#include "Model.hpp"


void Model::Draw(Shader &shader, Camera &camera){
    for(unsigned int i = 0; i < Meshes.size(); i++){ 
        Meshes[i].Draw(shader, camera);
    }
}

void Model::DrawMask(Shader &shader, Camera &camera, std::vector<GLuint> ID){
    for(unsigned int i = 0; i < Meshes.size(); i++){
        for (int j = 0; j < ID.size(); j++){
            if(Meshes[i].VAO.ID == ID[j]){
                Meshes[i].Draw(shader, camera);
            }
        }
        
    }
}

void Model::loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_BasePath + std::string(path), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    Directory = m_BasePath + std::string(path.substr(0, path.find_last_of('\\')));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene *scene){

    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
        }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene *scene){
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            // vertex
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName){
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for(unsigned int j = 0; j < TexturesLoaded.size(); j++)
        {
            if(TexturesLoaded[j].Path == str.C_Str())
            { 
                textures.push_back(TexturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        { 
            Texture texture;
            texture.ID = TextureFromFile(str.C_Str(), this->Directory);
            texture.Type = typeName;
            texture.Path = this->Directory + '\\' + std::string(str.C_Str());
            textures.push_back(texture);
            TexturesLoaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string directory, int* arg_width, int* arg_height){

    int tmpWidth, tmpHeight;

    int* width = arg_width  ? arg_width  : &tmpWidth;
    int* height = arg_height ? arg_height : &tmpHeight;

    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int nrComponents; 
    unsigned char *data = stbi_load(filename.c_str(), width, height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

glm::mat4 Model::CreateModelMatrix(const glm::vec3& position,
                            const glm::vec3& rotation,
                            const glm::vec3& scale) 
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position)
             * glm::toMat4(glm::quat(rotation))
             * glm::scale(glm::mat4(1.0f), scale);

    return model;
}