#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>

class Shader {
public:

    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    ~Shader();

    void Activate();
    void Delete();

    void setBool(const char* name, bool value) const;  
    void setInt(const char* name, int value) const;   
    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, const glm::vec3& value) const;  


private:
    bool isInitialized = false;
    const std::string _basePath = std::filesystem::current_path().string();
    void checkCompileErrors(unsigned int &shader, const char& type);
};

