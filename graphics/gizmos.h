#ifndef GIZMOS_H
#define GIZMOS_H

// File Description

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

struct Circle
{
  glm::vec3 color;
  glm::vec3 center;
  float size;
};

struct Gizmos
{
  size_t circle_points = 50;
  std::vector<Circle> circles;
};

void drawDot();
void drawLine();
void drawCube();
Circle CreateCircle(float size, glm::vec3 color, glm::vec3 pos, std::vector<Circle> &circles);
void UpdateCircles(double time_elapsed, std::vector<Circle> &circles);
void drawCapsule();
#endif // GIZMOS_H