#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <array>


enum BufferID
{
    BUFF_POINTS,
    BUFF_POINTS_STATIC,
    BUFF_LINES,
    BUFF_LINES_STATIC,
    BUFF_TRIANGLES,
    BUFF_CIRCLES,
    BUFF_CUBES,
    BUFF_WIREFRAME_CUBES,
    BUFF_SPHERES,
    BUFF_CAPSULES,
    BUFF_GROUND,
    BUFF_GRASS,
    BUFF_DEBUG,
    BUFF_LIGHT,
    BUFF_COUNT
};

enum AttributeID
{
    ATTR_POINTS,
    ATTR_SIZE,
    ATTR_SCALE,
    ATTR_COUNT
};

enum ShaderID
{
    SHADER_BASIC,
    SHADER_GIZMO,
    SHADER_SIZED,
    SHADER_SCALE,
    SHADER_GROUND,
    SHADER_GRASS,
    SHADER_DEBUG,
    SHADER_TRANSFORM,
    SHADER_COUNT
};

struct Graphics
{
    GLuint
        cameraUBO,
        lightSSBO;

    size_t max_points_static = 10000;
    size_t max_line_points_static = 10000;
    size_t max_points = 10000;
    size_t max_lines = 10000;
    size_t max_triangles = 10000;
    size_t max_circles = 50;
    size_t max_cubes_wireframe = 50;
    size_t max_cubes = 50;
    size_t max_spheres = 50;
    size_t max_capsules = 50;
    size_t max_planes = 64;
    size_t max_grass = 10000;
    size_t max_lights = 48;

    std::array<GLuint, SHADER_COUNT> shaders;
    std::array<GLuint, BUFF_COUNT> attribs;
    std::array<GLuint, BUFF_COUNT> buffers;
    GLuint noisetex;
};

namespace Gfx
{
    void CheckGLError(const char *functionName);

    //GLuint InitializeShaderFromPaths(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);
    std::string LoadShader(const std::filesystem::path &shaderPath, const std::filesystem::path &includePath);
    GLuint CompileShader(const std::filesystem::path &vertPath, const std::filesystem::path &fragPath);
    void Use(GLuint shaderID);
    void SetBool(GLuint shaderID, const std::string &name, bool value);
    void SetInt(GLuint shaderID, const std::string &name, int value);
    void SetFloat(GLuint shaderID, const std::string &name, float value);
    void SetMat3(GLuint shaderID, const std::string &name, const GLfloat *mat);
    void SetMat4(GLuint shaderID, const std::string &name, const GLfloat *mat);
    void SetVec2(GLuint shaderID, const std::string &name, glm::vec2 vec);
    void SetVec3(GLuint shaderID, const std::string &name, glm::vec3 vec);
    void SetVec4(GLuint shaderID, const std::string &name, glm::vec4 vec);

    GLuint CreateVAO();
    GLuint CreateVBO(GLsizeiptr verticesByteSize, const void *vertices, GLenum usage);
    void AddVertexAttrib(GLuint location, GLuint attribLen, GLuint stride, GLuint offset);
    GLuint CreateEBO(GLsizeiptr indicesByteSize, GLuint *indices, GLenum usage);
    void UseVAO(GLuint vaoID);
    void UnbindVAO();
};

#endif