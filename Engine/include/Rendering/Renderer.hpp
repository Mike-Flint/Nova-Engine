#pragma once
#include "Model.hpp"
#include "GState.hpp"

class Renderer{
private:
    Shader programShader;
    Shader maskShader;
    Shader postProcesShader;

    Model programModel;
    Model backpackModel;

    Camera camera;

    GLuint squareID;
    GLuint squareeID;

    glm::mat4 objectModel;

public:

    FBO viewportFBO;
    FBO maskFBO;
    FBO postFBO;
    Renderer();

    void draw(glm::ivec2 );
};