#include "gizmos.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

void CreateShapeSym(vec3 &pos, float size, vec4 &color, std::vector<ShapeSym> &shapes)
{
    ShapeSym shape{
        pos,
        size,
        color};

    shapes.push_back(shape);
    std::cout << "created shape" << std::endl;
}

void CreateShape(vec3 &pos, vec3 size, vec4 &color, std::vector<Shape> &shapes)
{
    Shape shape{
        pos,
        size,
        color};

    shapes.push_back(shape);
    std::cout << "created shape" << std::endl;
}
