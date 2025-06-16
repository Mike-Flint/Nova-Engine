#pragma once
#include "Model.h"
#include "GState.h"

class Renderer{
private:
    Shader programShader;
    Shader maskShader;
    Shader postProcesShader;

    Model programModel;
    Model backpackModel;

    Camera camera;

    GLuint squareID;

    glm::mat4 objectModel;

public:

    FBO viewportFBO;
    FBO maskFBO;
    FBO postFBO;
    Renderer();

    void draw();
};