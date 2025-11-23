#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "graphics.h"
#include "camera.h"

#include "global.h"

std::vector<glm::vec3> base_cube_wireframe;
std::vector<glm::vec3> base_cube;
std::vector<glm::vec3> base_circle;
std::vector<glm::vec3> base_sphere;
std::vector<glm::vec3> base_cylinder;
std::vector<glm::vec3> base_capsule;

void LoadBasics()
{

    int segments = 50;
    float size = 1.0f;
    float half = size / 2.0f;
    int rings = 12;

    // cube
    glm::vec3 cubeVerts[8] = {
        {-0.5f, -0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},
        {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},
        {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f}};

    // Edges (for wireframe)
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bottom
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4}, // top
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7} // sides
    };
    // Lines (for filled triangles cube)
    int cubeIndices[36] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 4, 7, 7, 3, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Bottom face
        0, 1, 5, 5, 4, 0,
        // Top face
        3, 2, 6, 6, 7, 3};

    for (int i = 0; i < 12; ++i)
    {
        base_cube_wireframe.push_back(cubeVerts[edges[i][0]]);
        base_cube_wireframe.push_back(cubeVerts[edges[i][1]]);
    }

    for (int i = 0; i < 36; ++i)
    {
        base_cube.push_back(cubeVerts[cubeIndices[i]]);
    }

    // circle
    for (int i = 0; i < segments; ++i)
    {
        float theta = (2.0f * glm::pi<float>() * i) / segments;
        base_circle.push_back(glm::vec3(cos(theta), sin(theta), 0.0f));
    }

    // sphere double halves for now
    for (int lat = 0; lat < rings / 2; ++lat)
    {
        float phi0 = glm::pi<float>() * lat / rings;
        float phi1 = glm::pi<float>() * (lat + 1) / rings;

        float z0 = cos(phi0) * size;
        float z1 = cos(phi1) * size;
        float r0 = sin(phi0) * size;
        float r1 = sin(phi1) * size;

        for (int lon = 0; lon < segments; ++lon)
        {
            float theta0 = 2.0f * glm::pi<float>() * lon / segments;
            float theta1 = 2.0f * glm::pi<float>() * (lon + 1) / segments;

            glm::vec3 v00(r0 * cos(theta0), r0 * sin(theta0), z0);
            glm::vec3 v01(r0 * cos(theta1), r0 * sin(theta1), z0);
            glm::vec3 v10(r1 * cos(theta0), r1 * sin(theta0), z1);
            glm::vec3 v11(r1 * cos(theta1), r1 * sin(theta1), z1);

            // First triangle
            base_sphere.push_back(v00);
            base_sphere.push_back(v10);
            base_sphere.push_back(v01);

            // Second triangle
            base_sphere.push_back(v10);
            base_sphere.push_back(v11);
            base_sphere.push_back(v01);
        }
    }

    for (int lat = 0; lat < rings / 2; ++lat)
    {
        float phi0 = glm::pi<float>() * lat / rings;
        float phi1 = glm::pi<float>() * (lat + 1) / rings;

        float z0 = -cos(phi0) * size;
        float z1 = -cos(phi1) * size;
        float r0 = sin(phi0) * size;
        float r1 = sin(phi1) * size;

        for (int lon = 0; lon < segments; ++lon)
        {
            float theta0 = 2.0f * glm::pi<float>() * lon / segments;
            float theta1 = 2.0f * glm::pi<float>() * (lon + 1) / segments;

            glm::vec3 v00(r0 * cos(theta0), r0 * sin(theta0), z0);
            glm::vec3 v01(r0 * cos(theta1), r0 * sin(theta1), z0);
            glm::vec3 v10(r1 * cos(theta0), r1 * sin(theta0), z1);
            glm::vec3 v11(r1 * cos(theta1), r1 * sin(theta1), z1);

            // First triangle
            base_sphere.push_back(v00);
            base_sphere.push_back(v10);
            base_sphere.push_back(v01);

            // Second triangle
            base_sphere.push_back(v10);
            base_sphere.push_back(v11);
            base_sphere.push_back(v01);
        }
    }


    // cylinder

    for (int i = 0; i < segments; ++i)
    {
        float theta0 = 2.0f * glm::pi<float>() * i / segments;
        float theta1 = 2.0f * glm::pi<float>() * (i + 1) / segments;

        float x0 = cos(theta0) * size;
        float y0 = sin(theta0) * size;
        float x1 = cos(theta1) * size;
        float y1 = sin(theta1) * size;

        glm::vec3 top0(x0, y0, half);
        glm::vec3 top1(x1, y1, half);
        glm::vec3 bot0(x0, y0, -half);
        glm::vec3 bot1(x1, y1, -half);

        // verticals
        base_cylinder.push_back(bot0);
        base_cylinder.push_back(top0);

        // top ring
        base_cylinder.push_back(top0);
        base_cylinder.push_back(top1);

        // bottom ring
        base_cylinder.push_back(bot0);
        base_cylinder.push_back(bot1);
    }

    // capsule
    // another cylinder first

    for (int i = 0; i < segments; ++i)
    {
        float theta0 = 2.0f * glm::pi<float>() * i / segments;
        float theta1 = 2.0f * glm::pi<float>() * (i + 1) / segments;

        float x0 = cos(theta0) * size;
        float y0 = sin(theta0) * size;
        float x1 = cos(theta1) * size;
        float y1 = sin(theta1) * size;

        glm::vec3 top0(x0, y0, half);
        glm::vec3 top1(x1, y1, half);
        glm::vec3 bot0(x0, y0, -half);
        glm::vec3 bot1(x1, y1, -half);

        // verticals
        base_capsule.push_back(bot0);
        base_capsule.push_back(top0);

        // top ring
        base_capsule.push_back(top0);
        base_capsule.push_back(top1);

        // bottom ring
        base_capsule.push_back(bot0);
        base_capsule.push_back(bot1);
    }

    for (int lat = 0; lat < rings / 2; ++lat)
    {
        float phi0 = (glm::pi<float>() / 2) * lat / (rings / 2);
        float phi1 = (glm::pi<float>() / 2) * (lat + 1) / (rings / 2);

        float z0 = cos(phi0) * size + half;
        float z1 = cos(phi1) * size + half;
        float r0 = sin(phi0) * size;
        float r1 = sin(phi1) * size;

        for (int i = 0; i < segments; ++i)
        {
            float theta0 = 2.0f * glm::pi<float>() * i / segments;
            float theta1 = 2.0f * glm::pi<float>() * (i + 1) / segments;

            glm::vec3 p0(r0 * cos(theta0), r0 * sin(theta0), z0);
            glm::vec3 p1(r0 * cos(theta1), r0 * sin(theta1), z0);
            glm::vec3 p2(r1 * cos(theta0), r1 * sin(theta0), z1);

            // ring segment
            base_capsule.push_back(p0);
            base_capsule.push_back(p1);

            // vertical connection
            base_capsule.push_back(p0);
            base_capsule.push_back(p2);
        }
    }
    for (int lat = 0; lat < rings / 2; ++lat)
    {
        float phi0 = (glm::pi<float>() / 2) * lat / (rings / 2);
        float phi1 = (glm::pi<float>() / 2) * (lat + 1) / (rings / 2);

        float z0 = -cos(phi0) * size - half;
        float z1 = -cos(phi1) * size - half;
        float r0 = sin(phi0) * size;
        float r1 = sin(phi1) * size;

        for (int i = 0; i < segments; ++i)
        {
            float theta0 = 2.0f * glm::pi<float>() * i / segments;
            float theta1 = 2.0f * glm::pi<float>() * (i + 1) / segments;

            glm::vec3 p0(r0 * cos(theta0), r0 * sin(theta0), z0);
            glm::vec3 p1(r0 * cos(theta1), r0 * sin(theta1), z0);
            glm::vec3 p2(r1 * cos(theta0), r1 * sin(theta0), z1);

            base_capsule.push_back(p0);
            base_capsule.push_back(p1);
            base_capsule.push_back(p0);
            base_capsule.push_back(p2);
        }
    }
}

#include <filesystem>

void SetupShaders(Graphics &graphics, Gizmos &gizmos, Camera &camera)
{

    LoadBasics();

    // Create all the shader programs:
    graphics.vertexID = Gfx::InitializeShader("vert.glsl", "frag.glsl");
    graphics.circleID = Gfx::InitializeShader("circle.glsl", "frag.glsl");
    graphics.wireframe_cubeID = Gfx::InitializeShader("circle.glsl", "frag.glsl");
    graphics.cubeID = Gfx::InitializeShader("circle.glsl", "frag.glsl");
    graphics.sphereID = Gfx::InitializeShader("circle.glsl", "frag.glsl");
    graphics.capID = Gfx::InitializeShader("capsule.glsl", "frag.glsl");

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

    graphics.max_points = std::max<size_t>(256, gizmos.points.capacity());
    graphics.max_line_points_static = std::max<size_t>(1024, gizmos.static_line_points.capacity());
    graphics.max_circles = std::max<size_t>(64, gizmos.circles.capacity());
    graphics.max_cubes_wireframe = std::max<size_t>(64, gizmos.wireframe_cubes.capacity());
    graphics.max_cubes = std::max<size_t>(64, gizmos.cubes.capacity());
    graphics.max_spheres = std::max<size_t>(64, gizmos.spheres.capacity());

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1.0f);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error here: 0x" << std::hex << err << std::dec << std::endl;
    }
};