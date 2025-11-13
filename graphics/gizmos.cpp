#include "gizmos.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

Gizmos gizmos;

Cube CreateCube(glm::vec3 pos, float size, glm::vec3 color, std::vector<Cube> &cubes)
{
  Cube cube{
      pos,
      size,
      color};

  cubes.push_back(cube);
  std::cout << "created cube" << std::endl;
  return cube;
}

Circle CreateCircle(glm::vec3 pos, float size, glm::vec3 color, std::vector<Circle> &circles)
{
  Circle circle{
      pos,
      size,
      color};

  circles.push_back(circle);
  std::cout << "created circle" << std::endl;
  return circle;
}

void drawCapsule()
{
}
