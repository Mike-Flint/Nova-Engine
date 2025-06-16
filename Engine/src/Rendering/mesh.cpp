#include "Mesh.h"

Mesh::Mesh( std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures){

    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    // Створення VAO (об'єкт вершинного масиву).
    VAO.Bind();

    // Створення VBO (буфер для вершин).
    VBO VBO(vertices);

    // Створення EBO (буфер для індексів).
    EBO EBO(indices);

    // Зв'язування даних атрибутів.
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // Позиції.
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // Кольори.
    VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // Координати текстур.
    VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    VAO.LinkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    // Відключення прив'язки.
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

    for(unsigned int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);

        std::string num;
        std::string type = textures[i].type;
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
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    camera.matrix(shader, "camMatrix");

    glBindVertexArray(VAO.ID);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
