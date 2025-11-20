#ifndef GIZMOS_H
#define GIZMOS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Point
{
  glm::vec3 pos;
  glm::vec3 color;
};

struct Line
{
  glm::vec3 start;
  glm::vec3 end;
  glm::vec3 color;
};

struct Cube
{
  glm::vec3 center;
  float size;
  glm::vec3 color;
};

struct Circle
{
  glm::vec3 center;
  float size;
  glm::vec3 color;
};

struct Sphere
{
  glm::vec3 center;
  float size;
  glm::vec3 color;
};

/* 
struct Capsule
{
  glm::vec3 center;
  float width;
  float height;
  glm::vec3 color;
};
 */

struct Capsule
{
    glm::vec4 center; // x, y, z, unused
    glm::vec4 size;   // x, y, z, unused
    glm::vec4 color;  // rgba
};

struct Gizmos
{
  size_t circle_points = 50;
  std::vector<Point> static_points;
  std::vector<Point> static_line_points;
  std::vector<Point> points;
  std::vector<Line> lines;
  std::vector<Point> line_points;
  std::vector<Cube> wireframe_cubes;
  std::vector<Cube> cubes;
  std::vector<Circle> circles;
  std::vector<Sphere> spheres;
  std::vector<Capsule> capsules;
};

void LoadBasics();

Cube CreateCube(glm::vec3 pos, float size, glm::vec3 color, std::vector<Cube> &cubes);
Circle CreateCircle(glm::vec3 pos, float size, glm::vec3 color, std::vector<Circle> &circles);

void UpdateCircles(double time_elapsed, std::vector<Circle> &circles);
void drawCapsule();
#endif // GIZMOS_H