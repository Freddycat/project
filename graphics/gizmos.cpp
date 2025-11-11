#include "gizmos.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

void drawDot()
{
}
void drawLine()
{
}
void drawCube()
{
}
Circle CreateCircle(float size, glm::vec3 color, glm::vec3 pos, std::vector<Circle> &circles)
{
  std::cout << "creating circle" << std::endl;

  Circle circle;
  circle.center = pos;
  circle.size = size;
  circle.color = color;

  circles.push_back(circle);
  std::cout << "created circle" << std::endl;
  return circle;
}

void drawCapsule()
{
}
