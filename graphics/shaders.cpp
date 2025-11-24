#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "graphics.h"
#include "camera.h"

#include "global.h"

#include <filesystem>

void SetupShaders(Graphics &graphics, Gizmos &gizmos, Camera &camera)
{

    LoadBasics();

    std::filesystem::path shaderpath = g.home / "shaders";

    // Create all the shader programs:
    graphics.vertexID = Gfx::InitializeShader(shaderpath / "vert.glsl", shaderpath / "frag.glsl");
    graphics.circleID = Gfx::InitializeShader(shaderpath / "circle.glsl", shaderpath / "frag.glsl");
    graphics.wireframe_cubeID = Gfx::InitializeShader(shaderpath / "circle.glsl", shaderpath / "frag.glsl");
    graphics.cubeID = Gfx::InitializeShader(shaderpath / "circle.glsl", shaderpath / "frag.glsl");
    graphics.sphereID = Gfx::InitializeShader(shaderpath / "circle.glsl", shaderpath / "frag.glsl");
    graphics.capID = Gfx::InitializeShader(shaderpath / "capsule.glsl", shaderpath / "frag.glsl");
    graphics.groundID = Gfx::InitializeShader(shaderpath / "groundv.glsl", shaderpath / "groundf.glsl");

    Gfx::CheckGLError("Gfx::use"); // this is just an error check, not sure if it works.

    // Create a global camera "uniform buffer" to use
    glGenBuffers(1, &graphics.cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, graphics.cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_DYNAMIC_DRAW); // allocates the 2 mat4
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, graphics.cameraUBO);                       // binds to "0" from layout
    glBindBuffer(GL_UNIFORM_BUFFER, 0);                                               // unbind
    // continue:

    gizmos.points.reserve(1000);
    gizmos.static_line_points.reserve(1000);
    gizmos.lines.reserve(1000);
    gizmos.circles.reserve(50);
    gizmos.cubes.reserve(50);
    gizmos.ground.reserve(64);

    graphics.max_points = std::max<size_t>(256, gizmos.points.capacity());
    graphics.max_line_points_static = std::max<size_t>(1024, gizmos.static_line_points.capacity());
    graphics.max_circles = std::max<size_t>(64, gizmos.circles.capacity());
    graphics.max_cubes_wireframe = std::max<size_t>(64, gizmos.wireframe_cubes.capacity());
    graphics.max_cubes = std::max<size_t>(64, gizmos.cubes.capacity());
    graphics.max_spheres = std::max<size_t>(64, gizmos.spheres.capacity());
    graphics.max_ground = std::max<size_t>(64, gizmos.ground.capacity());

    // -- POINTS (STATIC) --
    glGenVertexArrays(1, &graphics.vao_point);
    glBindVertexArray(graphics.vao_point);
    glGenBuffers(1, &graphics.vbo_point);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_point);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_points * sizeof(Point), gizmos.points.data(), GL_DYNAMIC_DRAW);
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, pos));
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, color));
    glEnableVertexAttribArray(1);
    glPointSize(6.0f);
    glBindVertexArray(0);

    // -- LINES --
    glGenVertexArrays(1, &graphics.vao_line);
    glBindVertexArray(graphics.vao_line);
    glGenBuffers(1, &graphics.vbo_line);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_line);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_line_points_static * sizeof(Point), gizmos.lines.data(), GL_DYNAMIC_DRAW);
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, pos));
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    if (glIsProgram(graphics.circleID))
        glUseProgram(graphics.circleID);
    else
        std::cerr << "Invalid shader program\n";

    Gfx::CheckGLError("Gfx::use");

    // -- CUBES -- (wireframe static)

    glGenVertexArrays(1, &graphics.vao_wireframecube);
    glBindVertexArray(graphics.vao_wireframecube);
    glGenBuffers(1, &graphics.vbo_wireframecube_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_wireframecube_buf);
    glBufferData(GL_ARRAY_BUFFER, base_cube_wireframe.size() * sizeof(glm::vec3), base_cube_wireframe.data(), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);
    // centered (instance)
    glGenBuffers(1, &graphics.vbo_wireframecubes);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_wireframecubes);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_cubes * sizeof(Cube), nullptr, GL_DYNAMIC_DRAW);
    // center
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, center));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    // scale
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // center changes per instance
    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // scale changes per instance

    glBindVertexArray(0);

    // -- CUBES -- (solid)

    glGenVertexArrays(1, &graphics.vao_cube);
    glBindVertexArray(graphics.vao_cube);
    glGenBuffers(1, &graphics.vbo_cube_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_cube_buf);
    glBufferData(GL_ARRAY_BUFFER, base_cube.size() * sizeof(glm::vec3), base_cube.data(), GL_STATIC_DRAW);
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);
    // centered (instance)
    glGenBuffers(1, &graphics.vbo_cubes);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_cubes);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_cubes * sizeof(Cube), nullptr, GL_DYNAMIC_DRAW);
    // center
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, center));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    // scale
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // center changes per instance
    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Cube), (void *)offsetof(Cube, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // scale changes per instance

    glBindVertexArray(0);

    // -- CIRCLES --

    if (glIsProgram(graphics.circleID))
        glUseProgram(graphics.circleID);
    else
        std::cerr << "Invalid shader program\n";

    Gfx::CheckGLError("Gfx::use");

    glGenVertexArrays(1, &graphics.vao_circle);
    glBindVertexArray(graphics.vao_circle);

    glGenBuffers(1, &graphics.vbo_circle_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_circle_buf);
    glBufferData(GL_ARRAY_BUFFER, base_circle.size() * sizeof(glm::vec3), base_circle.data(), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    // centered (instance)
    glGenBuffers(1, &graphics.vbo_circles);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_circles);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_circles * sizeof(Circle), nullptr, GL_DYNAMIC_DRAW);

    // center
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, center));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    // size
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

    // -- SPHERES --

    if (glIsProgram(graphics.circleID))
        glUseProgram(graphics.circleID);
    else
        std::cerr << "Invalid shader program\n";

    Gfx::CheckGLError("Gfx::use");

    glGenVertexArrays(1, &graphics.vao_sphere);
    glBindVertexArray(graphics.vao_sphere);

    glGenBuffers(1, &graphics.vbo_sphere_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_sphere_buf);
    glBufferData(GL_ARRAY_BUFFER, base_sphere.size() * sizeof(glm::vec3), base_sphere.data(), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    // centered (instance)
    glGenBuffers(1, &graphics.vbo_spheres);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_spheres);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_spheres * sizeof(Sphere), nullptr, GL_DYNAMIC_DRAW);

    // center
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere), (void *)offsetof(Sphere, center));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // center changes per instance

    // scale
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere), (void *)offsetof(Sphere, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // scale changes per instance

    // color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere), (void *)offsetof(Sphere, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

    // -- CAPSULES --

    if (glIsProgram(graphics.capID))
        glUseProgram(graphics.capID);
    else
        std::cerr << "Invalid shader program\n";

    Gfx::CheckGLError("Gfx::use");

    glGenVertexArrays(1, &graphics.vao_cap);
    glBindVertexArray(graphics.vao_cap);

    glGenBuffers(1, &graphics.vbo_cap_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_cap_buf);
    glBufferData(GL_ARRAY_BUFFER, base_capsule.size() * sizeof(glm::vec3), base_capsule.data(), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    // centered (instance)
    glGenBuffers(1, &graphics.vbo_caps);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_caps);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_capsules * sizeof(Capsule), nullptr, GL_DYNAMIC_DRAW);

    // center
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Capsule), (void *)offsetof(Capsule, center));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // center changes per instance

    // scale
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Capsule), (void *)offsetof(Capsule, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // scale changes per instance

    // color
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Capsule), (void *)offsetof(Capsule, color));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);

    // -- GROUND --

    if (glIsProgram(graphics.groundID))
        glUseProgram(graphics.groundID);
    else
        std::cerr << "Invalid shader program\n";

    Gfx::CheckGLError("Gfx::use");

    glGenVertexArrays(1, &graphics.vao_ground);
    glBindVertexArray(graphics.vao_ground);

    glGenBuffers(1, &graphics.vbo_ground_buf);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_ground_buf);
    glBufferData(GL_ARRAY_BUFFER, base_ground.size() * sizeof(glm::vec3), base_ground.data(), GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    // centered (instance)
    glGenBuffers(1, &graphics.vbo_ground);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_ground);
    glBufferData(GL_ARRAY_BUFFER, graphics.max_ground * sizeof(Ground), nullptr, GL_DYNAMIC_DRAW);

    // center
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Ground), (void *)offsetof(Ground, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // center changes per instance

    // scale
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Ground), (void *)offsetof(Ground, size));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // scale changes per instance

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0f);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error here: 0x" << std::hex << err << std::dec << std::endl;
    }
};