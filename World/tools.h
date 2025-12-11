#ifndef TOOLS_H
#define TOOLS_H

#include <glm/glm.hpp>

struct Selector
{
    size_t ID = 0;
    bool dragging = false;
    glm::vec3 offset{0.0};
};

#endif