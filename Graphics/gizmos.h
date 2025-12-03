#ifndef GIZMOS_H
#define GIZMOS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec2;
using glm::vec4;
using std::vector;

struct Point
{
    vec3 pos;
    vec4 color;
};

struct Line
{
    Point start;
    Point end;
};

struct ShapeSym
{
    vec3 center;
    float size;
    vec4 color;
};

struct Shape
{
    vec3 center;
    vec3 size;
    vec4 color;
};

struct GrassBlade
{
    vec3 pos;
    vec3 size;
    vec4 color;
};

struct Vertex_Basic
{
    vec3 pos;
    vec2 uv;
};

struct Plane
{
    glm::mat4 transform;
};

struct Gizmos
{
    size_t circle_points = 50;
    vector<Point> static_points;
    vector<Line> static_lines;
    vector<Point> points;
    vector<Line> lines;
    vector<Point> line_points;
    vector<Point> static_line_points;
    vector<ShapeSym> wireframe_cubes;
    vector<ShapeSym> cubes;
    vector<ShapeSym> circles;
    vector<ShapeSym> spheres;
    vector<Shape> capsules;
    vector<Shape> triangles;
    vector<Plane> plane;
    vector<GrassBlade> grass;
};

void LoadBasics();

void CreateShapeSym(vec3 &pos, float size, vec4 &color, vector<ShapeSym> &shapes);
void CreateShape(vec3 &pos, vec3 size, vec4 &color, vector<Shape> &shapes);

void drawCapsule();
#endif // GIZMOS_H