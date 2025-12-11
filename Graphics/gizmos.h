#ifndef GIZMOS_H
#define GIZMOS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Models/models.h"
#include "shapes.h"
#include "lights.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::vector;

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
    Model model;
    Sun sun;
};

void LoadBasics();

void CreateShape(vec3 &pos, vec3 size, vec4 &color, vector<Shape> &shapes);

void drawCapsule();
#endif // GIZMOS_H