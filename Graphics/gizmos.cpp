#include "gizmos.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <print>

void CreateShape(vec3 &pos, vec3 size, vec4 &color, std::vector<Shape> &shapes)
{
    Shape shape{
        pos,
        size,
        color};

    shapes.push_back(shape);
    std::print("Created shape:\npos: {} size: {}\n", glm::to_string(pos), glm::to_string(size));
}
