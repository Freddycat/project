
#include <iostream>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "render.h"
#include "shaders.h"
#include "primitives.h"
#include "player.h"
#include "world.h"
#include "app.h"

void render(float delta, Graphics &graphics, Camera &camera, Gizmos &gizmos, WorldCtx &ctx, World &world, Player &player, Gui &gui, Input &input, entt::registry &colliders)
{

    std::string time_str = formatTime();

    glClearColor(0.0, 0.1, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -- IMGUI --

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (g.console_on)
    {
        gui.DrawImGui(time_str, graphics, gizmos, player, camera, ctx, world, input, gizmos.lines, colliders);
    }
    else
        gui.DrawGameInfo(camera, ctx, gizmos.lines);

    if (gizmos.points.size() > graphics.max_points * 0.8f)
    {
        graphics.max_points = gizmos.points.size() * 1.5f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_points * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.lines.size() > graphics.max_lines * 0.8f)
    {
        graphics.max_lines = gizmos.lines.size() * 1.5f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_lines * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.circles.size() > graphics.max_circles * 0.8f)
    {
        graphics.max_circles = gizmos.circles.size() * 1.25f;
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CIRCLES]); // IMPORTANT
        glBufferData(GL_ARRAY_BUFFER, graphics.max_circles * sizeof(ShapeSym), nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.grass.size() > graphics.max_grass * 0.8f)
    {
        graphics.max_grass = gizmos.grass.size() * 1.5f;
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_GRASS]); // IMPORTANT
        glBufferData(GL_ARRAY_BUFFER, graphics.max_grass * sizeof(GrassBlade), nullptr, GL_DYNAMIC_DRAW);
    }

    /*
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam)
                           { std::cout << "GL DEBUG: " << message << std::endl; },
                           nullptr);

 */
    // -- CAMERA (universal) --
    glm::mat4 view = camera.view;
    glm::mat4 proj = camera.projection;

    glBindBuffer(GL_UNIFORM_BUFFER, graphics.cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // -- POINTS --
    Gfx::Use(graphics.shaders[SHADER_VERTS]);
    Gfx::UseVAO(graphics.attribs[BUFF_POINTS]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_POINTS]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.points.size() * sizeof(Point), gizmos.points.data());
    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, gizmos.points.size());
    Gfx::UnbindVAO();

    // -- LINES (STATIC) --

    Gfx::UseVAO(graphics.attribs[BUFF_LINES_STATIC]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_LINES_STATIC]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.static_line_points.size() * sizeof(Point), gizmos.static_line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.static_line_points.size());
    Gfx::UnbindVAO();

    // -- LINES (DYNAMIC) --
    for (auto &line : gizmos.lines)
    {
        gizmos.line_points.push_back(line.start);
        gizmos.line_points.push_back(line.end);
    }

    Gfx::UseVAO(graphics.attribs[BUFF_LINES]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_LINES]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.line_points.size() * sizeof(Point), gizmos.line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.line_points.size());
    Gfx::UnbindVAO();

    // -- CUBES --
    Gfx::Use(graphics.shaders[SHADER_SIZE]);
    Gfx::UseVAO(graphics.attribs[BUFF_CUBES]);
    if (!gizmos.cubes.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CUBES]);

        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.cubes.size() * sizeof(ShapeSym), gizmos.cubes.data());
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_cube.size(), gizmos.cubes.size());
    Gfx::UnbindVAO();

    // -- SPHERES --
    Gfx::Use(graphics.shaders[SHADER_SIZE]);
    Gfx::UseVAO(graphics.attribs[BUFF_SPHERES]);

    if (!gizmos.spheres.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_SPHERES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.spheres.size() * sizeof(ShapeSym), gizmos.spheres.data());
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_sphere.size(), gizmos.spheres.size());
    Gfx::UnbindVAO();

    // -- CIRCLES --
    Gfx::Use(graphics.shaders[SHADER_SIZE]);
    Gfx::UseVAO(graphics.attribs[BUFF_CIRCLES]);

    if (!gizmos.circles.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CIRCLES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.circles.size() * sizeof(ShapeSym), gizmos.circles.data());
    }
    glDrawArraysInstanced(GL_LINE_LOOP, 0, base_circle.size(), gizmos.circles.size());
    Gfx::UnbindVAO();

    // -- CAPSULES --
    Gfx::Use(graphics.shaders[SHADER_SCALE]);
    Gfx::UseVAO(graphics.attribs[BUFF_CAPSULES]);

    if (!gizmos.capsules.empty())
    {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CAPSULES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.capsules.size() * sizeof(Shape), gizmos.capsules.data());
    }

    glDrawArraysInstanced(GL_LINES, 0, base_capsule.size(), gizmos.capsules.size());
    Gfx::UnbindVAO();

    // -- GROUND --
    /*
        Gfx::Use(graphics.shaders[SHADER_GROUND]);
        std::cout << "using ground" << std::endl;
        Gfx::UseVAO(graphics.attribs[BUFF_GROUND]);
        std::cout << "using ground" << std::endl;
        GLint loc = glGetUniformLocation(graphics.shaders[BUFF_GROUND], "model");
        std::cout << "ground model" << std::endl;
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(gizmos.ground_model));
        std::cout << "ground model matrix" << std::endl;

        // update time uniform
        glUniform1f(glGetUniformLocation(graphics.shaders[BUFF_GROUND], "u_time"), delta);

        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        gizmos.ground.size() * sizeof(Ground),
                        gizmos.ground.data());
        glDrawArraysInstanced(GL_TRIANGLES, 0, base_ground.size(), gizmos.ground.size());
        Gfx::UnbindVAO();
     */

    // -- GRASS --
    Gfx::Use(graphics.shaders[SHADER_GRASS]);
    Gfx::UseVAO(graphics.attribs[BUFF_GRASS]);

    if (!gizmos.grass.empty())
    {
        Gfx::SetFloat(graphics.shaders[SHADER_GRASS], "time", g.game_time);
        Gfx::SetVec3(graphics.shaders[SHADER_GRASS], "characterPos", player.pos);
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_GRASS]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.grass.size() * sizeof(GrassBlade), gizmos.grass.data());
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_triangle.size(), gizmos.grass.size());
    Gfx::UnbindVAO();
/* 
    // -- DEBUG --
    Gfx::Use(graphics.shaders[SHADER_DEBUG]);
    Gfx::UseVAO(graphics.attribs[BUFF_DEBUG]);

    // update time uniform
    // glUniform1f(glGetUniformLocation(graphics.shaders[BUFF_DEBUG], "time"), delta);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_DEBUG]);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    gizmos.plane.size() * sizeof(Plane),
                    gizmos.plane.data());

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_ground.size(), gizmos.plane.size());
    Gfx::UnbindVAO();
 */
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(g.window);
}