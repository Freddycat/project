
#include <iostream>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "render.h"
#include "shaders.h"
#include "world.h"
#include "app.h"

void render(Graphics &graphics, Camera &camera, Gizmos &gizmos, World &world, Player &player, Gui &gui, entt::registry &registry, entt::entity &weapon, Input &input)
{

    std::string time_str = formatTime();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gizmos.points.size() > graphics.max_points)
    {
        graphics.max_points = gizmos.points.size() * 1.25f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_points * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
    }

    if (gizmos.lines.size() > graphics.max_lines)
    {
        graphics.max_lines = gizmos.lines.size() * 1.25f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_lines * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
    }

    if (gizmos.circles.size() > graphics.max_circles)
    {
        graphics.max_circles = gizmos.circles.size() * 1.25f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_circles * sizeof(Circle), nullptr, GL_DYNAMIC_DRAW);
    }

    // -- CAMERA (universal) --
    glm::mat4 view = camera.view;
    glm::mat4 proj = camera.projection;

    glBindBuffer(GL_UNIFORM_BUFFER, graphics.cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // -- POINTS --
    glUseProgram(graphics.vertexID);

    glBindVertexArray(graphics.vao_point);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_point);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.points.size() * sizeof(Point), gizmos.points.data());
    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, gizmos.points.size());
    glBindVertexArray(0);

    glm::vec3 test_color = gizmos.points[0].color;
    glm::vec3 test_pos = gizmos.points[0].pos;

    // -- LINES (STATIC) --
    glBindVertexArray(graphics.vao_line);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_line);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.static_line_points.size() * sizeof(Point), gizmos.static_line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.static_line_points.size());
    glBindVertexArray(0);

    // -- LINES (DYNAMIC) --
    for (auto &line : gizmos.lines)
    {
        Point a{line.start, line.color};
        Point b{line.end, line.color};
        gizmos.line_points.push_back(a);
        gizmos.line_points.push_back(b);
    }

    glBindVertexArray(graphics.vao_line);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_line);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.line_points.size() * sizeof(Point), gizmos.line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.line_points.size());
    glBindVertexArray(0);

    // -- CUBES --
    glUseProgram(graphics.wireframe_cubeID);
    glBindVertexArray(graphics.vao_wireframecube);

    if (!gizmos.cubes.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_wireframecubes);

        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.cubes.size() * sizeof(Cube), gizmos.cubes.data());
    }
    glLineWidth(4.0f);
    glDrawArraysInstanced(GL_LINES, 0, base_cube_wireframe.size(), gizmos.cubes.size());
    glBindVertexArray(0);

    glUseProgram(graphics.cubeID);
    glBindVertexArray(graphics.vao_cube);

    if (!gizmos.cubes.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_cubes);

        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.cubes.size() * sizeof(Cube), gizmos.cubes.data());
    }
    glDrawArraysInstanced(GL_TRIANGLES, 0, base_cube.size(), gizmos.cubes.size());
    glBindVertexArray(0);

    glUseProgram(graphics.cubeID);
    glBindVertexArray(graphics.vao_wireframecube);

    if (!gizmos.cubes.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_wireframecubes);

        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.cubes.size() * sizeof(Cube), gizmos.cubes.data());
    }
    glLineWidth(4.0f);
    glDrawArraysInstanced(GL_LINES, 0, base_cube_wireframe.size(), gizmos.cubes.size());
    glBindVertexArray(0);

    // -- CIRCLES --
    glUseProgram(graphics.circleID);
    glBindVertexArray(graphics.vao_circle);

    if (!gizmos.circles.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_circles);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.circles.size() * sizeof(Circle), gizmos.circles.data());
    }
    glDrawArraysInstanced(GL_LINE_LOOP, 0, base_circle.size(), gizmos.circles.size());
    glBindVertexArray(0);

    // -- CAPSULES --
    glUseProgram(graphics.capID);
    glBindVertexArray(graphics.vao_cap);

    if (!gizmos.capsules.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.vbo_caps);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.capsules.size() * sizeof(Capsule), gizmos.capsules.data());
    }

    glDrawArraysInstanced(GL_LINES, 0, base_capsule.size(), gizmos.capsules.size());
    glBindVertexArray(0);

    if (g.console_on)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        gui.DrawImGui(time_str, graphics, gizmos, player, camera, world, registry, weapon, input);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    SDL_GL_SwapWindow(g.window);
}