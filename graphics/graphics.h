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
        cameraUBO,
        
        vertexID, 
        vao_point, vbo_point, // points
        
        vao_line, vbo_line, // lines

        wireframe_cubeID,
        vao_wireframecube,
        vbo_wireframecube_buf,
        vbo_wireframecubes,

        circleID, // circles
        vao_circle,
        vbo_circle_buf,
        vbo_circles,

        // SOLID OBJECTS

        sphereID, //solid spheres
        vao_sphere,
        vbo_sphere_buf,
        vbo_spheres,

        cubeID, //solid cubes
        vao_cube,
        vbo_cube_buf,
        vbo_cubes,

        capID, // capsules
        vao_cap,
        vbo_cap_buf,
        vbo_caps,

        groundID, // the ground
        vao_ground,
        vbo_ground_buf,
        vbo_ground;

    size_t max_points_static = 10000;
    size_t max_line_points_static = 10000;
    size_t max_points = 10000;
    size_t max_lines = 10000;
    size_t max_circles = 50;
    size_t max_cubes_wireframe = 50;
    size_t max_cubes = 50;
    size_t max_spheres = 50;
    size_t max_capsules = 50;
    size_t max_ground = 64;
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