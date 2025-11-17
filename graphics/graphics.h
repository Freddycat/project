#define GRAPHICS_H
#ifdef GRAPHICS_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>

#include "gizmos.h"

struct Graphics
{
  GLuint
      vertexID, // points
      vao_point, vbo_point,
      // lines
      vao_line, vbo_line,

      cubeID,
      vao_cube,
      vbo_cube_buf,
      vbo_cubes,

      circleID, // circles
      vao_circle,
      vbo_circle_buf,
      vbo_circles,
      
      capID, // circles
      vao_cap,
      vbo_cap_buf,
      vbo_caps;

  size_t max_points_static = 10000;
  size_t max_line_points_static = 10000;
  size_t max_points = 10000;
  size_t max_lines = 10000;
  size_t max_circles = 50;
  size_t max_cubes = 50;
  size_t max_capsules = 50;
};

namespace Gfx
{
  void CheckGLError(const char *functionName);
  extern GLuint shaderID;
  GLuint InitializeShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);

  // if needed setup:
  void use(GLuint shaderID);
  void setBool(GLuint shaderID, const std::string &name, bool value);
  void setInt(GLuint shaderID, const std::string &name, int value);
  void setFloat(GLuint shaderID, const std::string &name, float value);
  void setMat3(GLuint shaderID, const std::string &name, const GLfloat *mat);
  void setMat4(GLuint shaderID, const std::string &name, const GLfloat *mat);
  void setVec2(GLuint shaderID, const std::string &name, glm::vec2 vec);
  void setVec3(GLuint shaderID, const std::string &name, glm::vec3 vec);
  void setVec4(GLuint shaderID, const std::string &name, glm::vec4 vec);
};

#endif