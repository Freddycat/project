
#include "graphics.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace Gfx
{
  void CheckGLError(const char *functionName)
  {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      std::cerr << "OpenGL Error in " << functionName << ": " << error << std::endl;
    }
  }
  GLuint shaderID = 0;
  GLuint InitializeShader(const std::string &vertexPath, const std::string &fragmentPath)
  {
    std::cout << "Vertex Shader Path: " << vertexPath << std::endl;
    std::cout << "Fragment Shader Path: " << fragmentPath << std::endl;
    // 1. Retrieve the vertex / fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (!std::filesystem::exists(vertexPath))
    {
      throw std::runtime_error("ERROR::SHADER::VERTEX_FILE_NOT_FOUND: " + vertexPath);
    }
    if (!std::filesystem::exists(fragmentPath))
    {
      throw std::runtime_error("ERROR::SHADER::FRAGMENT_FILE_NOT_FOUND: " + fragmentPath);
    }
    try
    {
      // Open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      // Read file's buffer contents into streams
      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // Close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // Convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
      throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }

    // 2. Compile shaders
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertexShaderId, fragmentShaderId;
    int success;
    char infoLog[512];

    // Create vertex shader
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);     // Create a vertex shader
    glShaderSource(vertexShaderId, 1, &vShaderCode, NULL); // Attach the vertex shader source code
    glCompileShader(vertexShaderId);                       // Compile the vertex shader
    // Check for vertex shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
      throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    // Create fragment shader
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);   // Create a fragment shader
    glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL); // Attach the fragment shader source code
    glCompileShader(fragmentShaderId);                       // Compile the fragment shader
    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
      glDeleteShader(vertexShaderId);
      throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }

    // Shader program
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderId);
    glAttachShader(programID, fragmentShaderId);
    glLinkProgram(programID);
    // Check for shader program linking errors
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(programID, 512, NULL, infoLog);
      std::cout << infoLog << std::endl;
      glDeleteShader(vertexShaderId);
      glDeleteShader(fragmentShaderId);
      throw std::runtime_error("ERROR::PROGRAM::LINKING_FAILED\n");
    }

    GLint count;
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
    std::cout << "Active attributes: " << count << std::endl;

    // Delete vertex and fragment shader instances as they have been linked
    glDetachShader(programID, vertexShaderId);
    glDetachShader(programID, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return programID;
  }

  void CheckGLError(const char *functionName);
  extern GLuint shaderID;
  GLuint InitializeShader(const std::string &vertexPath, const std::string &fragmentPath);

  // if needed setup:
  void use(GLuint shaderID)
  {
    glUseProgram(shaderID);
  }
  void setBool(GLuint shaderID, const std::string &name, bool value)
  {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
  }
  void setInt(GLuint shaderID, const std::string &name, int value)
  {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
  }
  void setFloat(GLuint shaderID, const std::string &name, float value)
  {
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
  }
  void setMat3(GLuint shaderID, const std::string &name, const GLfloat *mat)
  {
    glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
  }
  void setMat4(GLuint shaderID, const std::string &name, const GLfloat *mat)
  {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
  }
  void setVec2(GLuint shaderID, const std::string &name, glm::vec2 vec)
  {
    glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y);
  }
  void setVec3(GLuint shaderID, const std::string &name, glm::vec3 vec)
  {
    glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);
  }
  void setVec4(GLuint shaderID, const std::string &name, glm::vec4 vec)
  {
    glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
  }
}
