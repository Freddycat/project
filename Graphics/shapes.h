#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

struct Point {
    vec3 pos;
    vec4 color;
};

struct Line {
    Point start;
    Point end;
};

struct GrassBlade {
    vec3 pos;
    vec3 size;
    vec4 color;
};

struct Plane {
    glm::mat4 transform;
};

struct Shape {
    vec3 pos;
    vec3 size;
    vec4 color;
};

struct Transform {
    glm::mat4 transform;
};

#endif