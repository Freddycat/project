#include "meshes.h"
#include "Graphics.h"
#include "materials.h"
#include "primitives.h"
#include <glm/gtc/type_ptr.hpp>
#include <print>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "gizmos.h"
#include "addAttributes.h"

void MeshAddShapeAttribs(GLuint &buffer, int start = 0)
{
    int s = start;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Gfx::AddVertexAttrib(s + 1, 3, sizeof(Shape), offsetof(Shape, pos));
    glVertexAttribDivisor(s + 1, 1);
    Gfx::AddVertexAttrib(s + 2, 3, sizeof(Shape), offsetof(Shape, size));
    glVertexAttribDivisor(s + 2, 1);
    Gfx::AddVertexAttrib(s + 3, 4, sizeof(Shape), offsetof(Shape, color));
    glVertexAttribDivisor(s + 3, 1);
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Material material)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;

    std::print("Received color should be: {}\n", glm::to_string(material.diffuse));
    std::print("this-> color should be: {}\n", glm::to_string(this->material.diffuse));

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    //              &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    instance = Gfx::CreateVBO(
        max_players * sizeof(Transform), nullptr, GL_DYNAMIC_DRAW);
    AddTransformAttribs(instance, 2);

    glBindVertexArray(0);
}

void SetMaterial(GLuint &shader, Material &mat)
{
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(mat.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(mat.specular));
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(mat.ambient));
    glUniform1f(glGetUniformLocation(shader, "material.shine"), mat.shine);
    glUniform1i(glGetUniformLocation(shader, "material.useAmbiUV"), false);
    glUniform1i(glGetUniformLocation(shader, "material.useDiffUV"), false);
    glUniform1i(glGetUniformLocation(shader, "material.useSpecUV"), false);
}

void Mesh::Draw(GLuint &shader, Transform&transform)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    if (!textures.empty())
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

    Gfx::Use(shader);

    Gfx::UseVAO(VAO);

    SetMaterial(shader, material);
    
    glBindBuffer(GL_ARRAY_BUFFER, instance);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(Transform),
                    &transform);

    glDrawArraysInstanced(GL_TRIANGLES, 0, indices.size(), 1);
    Gfx::UnbindVAO();
    // draw mesh
    /*
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
 */
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}