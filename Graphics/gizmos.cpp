#include "gizmos.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <print>
/* 
void CreateShapeSym(vec3 &pos, float size, std::vector<ShapeSym> &shapes)
{
    ShapeSym shape{
        pos,
        size};

    shapes.push_back(shape);
    std::cout << "created shape" << std::endl;
}
 */
void CreateShape(vec3 &pos, vec3 size, vec4 &color, std::vector<Shape> &shapes)
{
    Shape shape{
        pos,
        size,
        color};

    shapes.push_back(shape);
    std::cout << "created shape" << std::endl;
    std::print("pos: {} size: {}\n", glm::to_string(pos), glm::to_string(size));
}
