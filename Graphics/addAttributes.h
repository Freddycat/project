#ifndef ADD_ATTRIBUTES_H
#define ADD_ATTRIBUTES_H

#include "Graphics.h"
#include "shapes.h"
#include <glad/glad.h>

inline void AddShapeAttribs(GLuint &buffer, int start = 0)
{
    int s = start;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Gfx::AddVertexAttrib(s + 1, 3, sizeof(Shape), offsetof(Shape, pos));
    Gfx::AddVertexAttrib(s + 2, 3, sizeof(Shape), offsetof(Shape, size));
    Gfx::AddVertexAttrib(s + 3, 4, sizeof(Shape), offsetof(Shape, color));
    glVertexAttribDivisor(s + 1, 1);
    glVertexAttribDivisor(s + 2, 1);
    glVertexAttribDivisor(s + 3, 1);
}

inline void AddTransformAttribs(GLuint &buffer, int start = 0)
{
    int s = start;
    auto vecSize = sizeof(glm::vec4);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    Gfx::AddVertexAttrib(s + 1, 4, sizeof(Transform), offsetof(Transform, transform));
    Gfx::AddVertexAttrib(s + 2, 4, sizeof(Transform), offsetof(Transform, transform) + 1 * vecSize);
    Gfx::AddVertexAttrib(s + 3, 4, sizeof(Transform), offsetof(Transform, transform) + 2 * vecSize);
    Gfx::AddVertexAttrib(s + 4, 4, sizeof(Transform), offsetof(Transform, transform) + 3 * vecSize);
    glVertexAttribDivisor(s + 1, 1);
    glVertexAttribDivisor(s + 2, 1);
    glVertexAttribDivisor(s + 3, 1);
    glVertexAttribDivisor(s + 4, 1);
}

#endif