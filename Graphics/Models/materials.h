#ifndef MATERIALS_H
#define MATERIALS_H

#include <glm/glm.hpp>
#include <string>

using glm::vec3;
using std::string;

struct Texture {
    unsigned int id;
    string type;
};

struct Material {
    // vec4 ambient = {0.2f, 0.2f, 0.2f, 0.0};
    // vec4 diffuse = {0.8f, 0.8f, 0.8f, 0.0};
    // vec4 specular = {0.2f, 0.2f, 0.2f, 0.0};
    vec3 ambient = vec3{1.0};
    vec3 diffuse = vec3{0.2, 0.2, 0.8};
    vec3 specular = vec3{1.0};
    vec3 emission = {0.0f, 0.0f, 0.0f};
    float shine = 64.0f;
    bool two_sided = false;
    bool wireframe = false;

    bool useAmbiUV = false;
    bool useDiffUV = false;
    bool useSpecUV = false;
};

#endif