#define GRAPHICS_H
#ifdef GRAPHICS_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gizmos.h"

struct Graphics
{
  GLuint vertexID, // points
      vao_point, vbo_point,
      vao_line, vbo_line,
      circleID, // circles
      vao_circle,
      vbo_circle_buf,
      vbo_circles;

  std::vector<Point> points;
  std::vector<Point> lines;

  size_t max_points = 10000;
  size_t max_lines = 10000;
  size_t max_circles = 50;
  
  std::vector<glm::vec3> base_circle;
};

namespace glGraphics
{
  void checkGLError(const char *functionName);
  namespace Shader
  {
    extern GLuint shaderID;
    GLuint InitializeShader(const std::string &vertexPath, const std::string &fragmentPath);
    void use(GLuint shaderProgram);
    void setBool(GLuint shaderProgram, const std::string &name, bool value);
    void setInt(GLuint shaderProgram, const std::string &name, int value);
    void setFloat(GLuint shaderProgram, const std::string &name, float value);
    void setMat3(GLuint shaderProgram, const std::string &name, const GLfloat *mat);
    void setMat4(GLuint shaderProgram, const std::string &name, const GLfloat *mat);
    void setVec2(GLuint shaderProgram, const std::string &name, glm::vec2 vec);
    void setVec3(GLuint shaderProgram, const std::string &name, glm::vec3 vec);
    void setVec4(GLuint shaderProgram, const std::string &name, glm::vec4 vec);
  }
  namespace Buffer
  {
    GLuint createVAO();
    GLuint createVBO(GLsizeiptr verticesByteSize, const void *vertices, GLenum usage);
    void addVertexAttrib(GLuint location, GLuint attribLen, GLuint stride, GLuint offset);
    GLuint createEBO(GLsizeiptr indicesByteSize, GLuint *indices, GLenum usage);
    void useVAO(GLuint vaoID);
    void unbindVAO();
  }
};

#endif