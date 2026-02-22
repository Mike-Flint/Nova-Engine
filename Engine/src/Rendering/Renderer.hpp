#pragma once
<<<<<<< HEAD
#include "Components.hpp"
=======
#include "Model.hpp"
>>>>>>> 209b5a9 (END)
#include "Scene.hpp"
#include <optional>

class Renderer{
private:


public:
    Scene *GScene;

    Shader maskShader;
    Shader postProcesShader;
    Shader programShader;

    std::vector<GLuint> SelectedObjects;

    FBO viewportFBO;
    FBO maskFBO;
    FBO postFBO;

    glm::mat4 objectModel;

    GLuint squareID;
    GLuint squareeID;
    Renderer(Scene* scene);

    void Draw();
};