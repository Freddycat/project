#ifndef MESHES_H
#define MESHES_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "materials.h"
#include "shapes.h"
#include "vertex.h"

using glm::vec2;
using glm::vec3;
using std::string;
using std::vector;
struct Shape;

class Mesh {
  public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Material material);
    void Draw(GLuint &shader, Transform &transform);

  private:
    //  render data
    unsigned int VAO, VBO, EBO;

    GLuint instance;

    void setupMesh();
};

#endif