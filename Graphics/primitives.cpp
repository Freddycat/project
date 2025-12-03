#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "primitives.h"
#include "camera.h"

#include "global.h"

vector<vec3> base_triangle;

vector<vec3> base_cube_wireframe;
vector<vec3> base_cube;
vector<vec3> base_circle;
vector<vec3> base_sphere;
vector<vec3> base_cylinder;
vector<vec3> base_capsule;
vector<vec3> base_ground;
vector<Vertex_Basic> base_plane;

void LoadBasics()
{
    int segments = 50;
    float size = 1.0f;
    float half = size / 2.0f;
    int rings = 12;

    vec3 triangle[3] = {
        vec3(-1.0f, 0.0f, 0.0f), // left bottom
        vec3(1.0f, 0.0f, 0.0f),  // right bottom
        vec3(0.0f, 0.0f, 1.0f)   // top
    };

    base_triangle.push_back(triangle[0]);
    base_triangle.push_back(triangle[1]);
    base_triangle.push_back(triangle[2]);

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

    // -- GROUND --

    int N = 64;
    float scale = 1.0f;

    for (int z = 0; z < N; z++)
    {
        for (int x = 0; x < N; x++)
        {

            glm::vec3 p0 = {x * scale, 0, z * scale};
            glm::vec3 p1 = {(x + 1) * scale, 0, z * scale};
            glm::vec3 p2 = {(x + 1) * scale, 0, (z + 1) * scale};
            glm::vec3 p3 = {x * scale, 0, (z + 1) * scale};

            base_ground.push_back(p0);
            base_ground.push_back(p1);
            base_ground.push_back(p2);
            base_ground.push_back(p0);
            base_ground.push_back(p2);
            base_ground.push_back(p3);
        }
    }
    
    std::vector<Vertex_Basic> plane = {
        {{-0.5, -0.5, 0}, {0, 0}},
        {{0.5, -0.5, 0}, {1, 0}},
        {{0.5, 0.5, 0}, {1, 1}},

        {{0.5, 0.5, 0}, {1, 1}},
        {{-0.5, 0.5, 0}, {0, 1}},
        {{-0.5, -0.5, 0}, {0, 0}}};

    base_plane = plane;
}