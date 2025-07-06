#pragma once
#include "Model.hpp"
#include "GState.hpp"

class Renderer{
private:
    bool isInitialized = false;

    Shader programShader;
    Shader maskShader;
    Shader postProcesShader;

    Model programModel;


    Camera camera;

    GLuint squareID;
    GLuint squareeID;

    glm::mat4 objectModel;

public:

    FBO viewportFBO;
    FBO maskFBO;
    FBO postFBO;
    Renderer();

    // void init();

    void draw(glm::ivec2 );
};