#ifndef GIZMOS_H
#define GIZMOS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Models/materials.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::vector;

struct Point {
    vec3 pos;
    vec4 color;
};

struct Line {
    Point start;
    Point end;
};

struct Shape {
    vec3 center;
    vec3 size;
    vec4 color;
};

struct GrassBlade {
    vec3 pos;
    vec3 size;
    vec4 color;
};

struct Plane {
    glm::mat4 transform;
};

struct Light {
    vec4 position = vec4{0.0};
    vec4 color = vec4{1.0};

    vec4 attenuation = vec4
    {
        1.0,
        0.01,
        0.012,
        0.0
    };

    vec4 ambient = vec4{1.0};
    vec4 diffuse = vec4{10.0};
    vec4 specular = vec4{10.0};
};

struct Sun {
    vec3 direction = vec3{0.01, 0.01, 1.0};
    vec3 ambient = vec4{0.2};
    vec3 diffuse = vec4{1.0};
    vec3 specular = vec4{1.0};
    
    //vec3 color = vec4{1.0};
};

struct Gizmos {
    size_t circle_points = 50;
    vector<Point> static_points;
    vector<Line> static_lines;
    vector<Point> points;
    vector<Line> lines;
    vector<Point> line_points;
    vector<Point> static_line_points;
    vector<Light> lights;
    vector<Shape> circles;
    vector<Shape> triangles;
    vector<Plane> planes;
    vector<Shape> cubes;
    vector<Shape> spheres;
    vector<Shape> capsules;
    vector<GrassBlade> grass;
    vector<Shape> wireframe_cubes;
    Sun sun;
};

void LoadBasics();

//void CreateShapeSym(vec3 &pos, float size, vector<ShapeSym> &shapes);
void CreateShape(vec3 &pos, vec3 size, vec4 &color, vector<Shape> &shapes);

void drawCapsule();
#endif // GIZMOS_H