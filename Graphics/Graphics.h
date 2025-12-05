#define GRAPHICS_H
#ifdef GRAPHICS_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <array>

#include "gizmos.h"

enum BufferID
{
    BUFF_POINTS,
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
    SHADER_VERTS,
    SHADER_SIZE,
    SHADER_SCALE,
    SHADER_GROUND,
    SHADER_GRASS,
    SHADER_DEBUG,
    SHADER_COUNT
};

struct Graphics
{
    GLuint
        cameraUBO,

        vert,
        vert_inst_size,
        vert_inst_transform,
        vert_ground,
        vert_grass,

        vao_point, vbo_point, // points

        vao_line, vbo_line, // lines
                            // grass
        vao_grass, vbo_grass_buff,
        vbo_grass,

        vao_wireframecube,
        vbo_wireframecube_buf,
        vbo_wireframecubes,

        vao_circle, // circles
        vbo_circle_buf,
        vbo_circles,

        // SOLID OBJECTS
        vao_sphere,
        vbo_sphere_buf,
        vbo_spheres,
        // solid cubes
        vao_cube,
        vbo_cube_buf,
        vbo_cubes,

        capID, // capsules
        vao_cap,
        vbo_cap_buf,
        vbo_caps,

        vao_ground,
        vbo_ground_buf,
        vbo_ground;

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
    size_t max_plane = 64;
    size_t max_grass = 10000;

    std::array<GLuint, SHADER_COUNT> shaders;
    std::array<GLuint, BUFF_COUNT> attribs;
    std::array<GLuint, BUFF_COUNT> buffers;
    GLuint noisetex;
};

namespace Gfx
{
    void CheckGLError(const char *functionName);
    extern GLuint shaderID;
    GLuint InitializeShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath);

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