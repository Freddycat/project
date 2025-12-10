#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "vertex.h"
#include "materials.h"

using std::vector;
using std::string;
using glm::vec3;
using glm::vec2;

class Mesh
{
public:
    // mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(GLuint &shader);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};