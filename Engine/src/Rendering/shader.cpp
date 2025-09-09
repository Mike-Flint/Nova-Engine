#include "Shader.hpp"
Shader::~Shader(){
    this->Delete();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath){
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;


    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (geometryPath != nullptr)
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    
    try {
        vShaderFile.open(_basePath + std::string(vertexPath));
        fShaderFile.open(_basePath + std::string(fragmentPath));
        if (geometryPath != nullptr)
            gShaderFile.open(_basePath + std::string(geometryPath));
        std::stringstream vShaderStream, fShaderStream, gShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        if (geometryPath != nullptr)
            gShaderStream << gShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        if (geometryPath != nullptr)
            gShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr)
            geometryCode = gShaderStream.str();
    }
    catch (std::ifstream::failure&) {
        std::cout << "ERROR::SHADER::FILE_NOT::SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char *gShaderCode;
    if (geometryPath == nullptr)
        gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, 'V');

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, 'F');

    if (geometryPath != nullptr){
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, 'G');
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, 'P');

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
}

void Shader::Activate() {
    glUseProgram(ID);
}


void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::SetBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::SetInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value); 
}

void Shader::SetFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetVec3(const char* name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void Shader::checkCompileErrors(unsigned int& shader, const char& type) {
    int success;
    char infoLog[1024];

    
    if (type == 'V' || type == 'F' || type == 'G') {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else if (type == 'P') {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        std::cout << "ERROR::UNKNOWN_SHADER_TYPE: " << type << std::endl;
    }
}


