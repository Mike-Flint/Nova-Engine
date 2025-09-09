#include "Mesh.hpp"

Mesh::Mesh( std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures){

    Mesh::Vertices = vertices;
    Mesh::Indices = indices;
    Mesh::Textures = textures;

    VAO.Bind();

    VBO VBO(vertices);

    EBO EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // Позиції.
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // Кольори.
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // Координати текстур.
    VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    VAO.LinkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera){
    shader.Activate();
    VAO.Bind();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int mormalNr = 1;
    unsigned int heightNr = 1;

    for(unsigned int i = 0; i < Textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);

        std::string num;
        std::string type = Textures[i].Type.data();
        if (type == "texture_diffuse"){
            num = std::to_string(diffuseNr++);
        }
        else if (type == "texture_specular"){
            num = std::to_string(specularNr++);
        }
        else if (type == "texture_normal"){
            num = std::to_string(mormalNr++);
        }
        else if (type == "texture_height"){
            num = std::to_string(heightNr++);
        }
        
        glUniform1i(glGetUniformLocation(shader.ID, (type + num).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].ID);
    }

    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glBindVertexArray(VAO.ID);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
