
#include "Graphics.h"
#include "global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <print>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include "stb_include.h"

using std::filesystem::path;

std::string Gfx::LoadShader(const path &shaderPath, const path &includePath)
{

    std::string shaderStr = shaderPath.string();
    std::string includeStr = includePath.string();

    // stupid stupid stupid
    std::vector<char> shaderChar(shaderStr.begin(), shaderStr.end());
    shaderChar.push_back('\0'); // null terminate
    // making a const char string for stb_include
    std::vector<char> includeChar(includeStr.begin(), includeStr.end());
    includeChar.push_back('\0'); // null terminate

    char error[256] = {0};

    char *result = stb_include_file(shaderChar.data(), nullptr, includeChar.data(), error);

    if (!result)
    {
        std::print("Failed to load shader: {}", error);
        return "";
    }
    std::string shaderSrc(result);
    free(result); // stb_include_file allocates with malloc
    return shaderSrc;
}

void Gfx::CheckGLError(const char *functionName)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::print("OpenGL Error in {}\n", functionName, ": ", error);
    }
}
// old paths way
GLuint Gfx::InitializeShaderFromPaths(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
{
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
        throw std::runtime_error("ERROR::SHADER::VERTEX_FILE_NOT_FOUND: " + vertexPath.string());
    }
    if (!std::filesystem::exists(fragmentPath))
    {
        throw std::runtime_error("ERROR::SHADER::FRAGMENT_FILE_NOT_FOUND: " + fragmentPath.string());
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
}

GLuint Gfx::CompileShader(const path &vertPath, const path &fragPath)
{

    path home = g.home / "shaders";

    std::print("Vertex Shader Path: {}\n", vertPath.string());
    std::print("Fragment Shader Path: {}\n", fragPath.string());

    std::string vertStr = LoadShader(vertPath, home);
    std::string fragStr = LoadShader(fragPath, home);

    //std::print("Vert str: {}\n", vertStr);
    //std::print("Frag str: {}\n", fragStr);

    const char *vertexCode = vertStr.c_str();
    const char *fragmentCode = fragStr.c_str();
    unsigned int vertexShaderId, fragmentShaderId;
    int success;
    char infoLog[512];

    // Create vertex shader
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);    // Create a vertex shader
    glShaderSource(vertexShaderId, 1, &vertexCode, NULL); // Attach the vertex shader source code
    glCompileShader(vertexShaderId);                      // Compile the vertex shader
    // Check for vertex shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }

    // Create fragment shader
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);    // Create a fragment shader
    glShaderSource(fragmentShaderId, 1, &fragmentCode, NULL); // Attach the fragment shader source code
    glCompileShader(fragmentShaderId);                        // Compile the fragment shader
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
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    // Check for shader program linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        throw std::runtime_error("ERROR::PROGRAM::LINKING_FAILED\n");
    }

    GLint count;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    std::print("Active attributes: {}\n", count);

    // Delete vertex and fragment shader instances as they have been linked
    glDetachShader(program, vertexShaderId);
    glDetachShader(program, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}

namespace Gfx
{

    void CheckGLError(const char *functionName);

    void Use(GLuint shader)
    {
        glUseProgram(shader);
    }

    void SetBool(GLuint shader, const std::string &name, bool value)
    {
        glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value);
    }

    void SetInt(GLuint shader, const std::string &name, int value)
    {
        glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
    }

    void SetFloat(GLuint shader, const std::string &name, float value)
    {
        glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
    }

    void SetMat3(GLuint shader, const std::string &name, const GLfloat *mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, mat);
    }

    void SetMat4(GLuint shader, const std::string &name, const GLfloat *mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, mat);
    }

    void SetVec2(GLuint shader, const std::string &name, glm::vec2 vec)
    {
        glUniform2f(glGetUniformLocation(shader, name.c_str()), vec.x, vec.y);
    }

    void SetVec3(GLuint shader, const std::string &name, glm::vec3 vec)
    {
        glUniform3f(glGetUniformLocation(shader, name.c_str()), vec.x, vec.y, vec.z);
    }

    void SetVec4(GLuint shader, const std::string &name, glm::vec4 vec)
    {
        glUniform4f(glGetUniformLocation(shader, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }

    GLuint CreateVAO()
    {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        return vao;
    }

    GLuint CreateVBO(GLsizeiptr verticesByteSize, const void *vertices, GLenum usage)
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize, vertices, usage);
        return vbo;
    }

    void AddVertexAttrib(GLuint location, GLuint attribLen, GLuint stride, GLuint offset)
    {
        glVertexAttribPointer(location, attribLen, GL_FLOAT, GL_FALSE, stride, (void *)(uintptr_t)offset);
        glEnableVertexAttribArray(location);
    }

    GLuint CreateEBO(GLsizeiptr indicesByteSize, GLuint *indices, GLenum usage)
    {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesByteSize, indices, usage);
        return ebo;
    }

    void UseVAO(GLuint vao)
    {
        glBindVertexArray(vao);
    }

    void UnbindVAO()
    {
        glBindVertexArray(0);
    }
}