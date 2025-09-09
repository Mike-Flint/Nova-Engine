#include "Renderer.hpp"


GLuint CreateSquareOnScreen(){
    VAO VAO;
    VAO.Bind();

    std::vector<Vertex> quadVertices = {
        {{-1.0f,  1.0f, 0.0f}, {}, {}, {0.0f, 1.0f}, {}},
        {{-1.0f, -1.0f, 0.0f}, {}, {}, {0.0f, 0.0f}, {}},
        {{ 1.0f, -1.0f, 0.0f}, {}, {}, {1.0f, 0.0f}, {}},
        {{ 1.0f,  1.0f, 0.0f}, {}, {}, {1.0f, 1.0f}, {}}
    };

    VBO VBO(quadVertices);

    std::vector<GLuint> quadIndices = {
        0, 1, 2, 
        0, 2, 3 
    };

    EBO EBO(quadIndices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
    return VAO.ID;
}

Renderer::Renderer(Scene* scene) : 
    programShader("\\Assets\\Shaders\\vertex_shader.vs",
                    "\\Assets\\Shaders\\fragment_shader.fs"),

    maskShader("\\Assets\\Shaders\\mask.vs",
                    "\\Assets\\Shaders\\mask.fs"),

    postProcesShader("\\Assets\\Shaders\\postProces.vs",
                        "\\Assets\\Shaders\\postProces.fs"),

    maskFBO(glm::ivec2(600, 600), GL_RED),
    postFBO(glm::ivec2(600, 600), GL_RGB, true),
    viewportFBO(glm::ivec2(600, 600), GL_RGB, true),
    GScene(scene)
    {

    stbi_set_flip_vertically_on_load(true);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 1.0f);
    objectModel = glm::mat4(1.0f);

    programShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(programShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(programShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(programShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    maskShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(maskShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

    squareID = CreateSquareOnScreen();

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
} 

void Renderer::Draw(){
    glEnable(GL_DEPTH_TEST);
    
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    glm::ivec2 vpSize = glm::ivec2(viewportSize.x, viewportSize.y);
    Camera *camera = GScene->GetComponentsOfType<Camera>()[0];
    Model *programModel = GScene->GetComponentsOfType<Model>()[0];

    
    if (vpSize.width > 1 || vpSize.height > 1){
        camera->UpdateSizeWindow(vpSize);
        maskFBO.UpdateSizeWindow(vpSize);
        postFBO.UpdateSizeWindow(vpSize);
        viewportFBO.UpdateSizeWindow(vpSize);
    }

    camera->Inputs();
    camera->UpdateMatrix();

    postFBO.Bind();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        programShader.Activate();
        programModel->Draw(programShader, *camera);
    postFBO.Unbind();

    maskFBO.Bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        maskShader.Activate();
        programModel->DrawMask(maskShader, *camera, SelectedObjects);
    maskFBO.Unbind();

    viewportFBO.Bind();
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        postProcesShader.Activate();
        glViewport(0, 0 , vpSize.width, vpSize.height);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, postFBO.TextureID);
        glUniform1i(glGetUniformLocation(postProcesShader.ID, "screenTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, maskFBO.TextureID);
        glUniform1i(glGetUniformLocation(postProcesShader.ID, "maskTexture"), 1);
        
        glBindVertexArray(squareID);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    viewportFBO.Unbind();
}


