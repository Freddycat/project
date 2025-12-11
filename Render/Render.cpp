
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "GUI.h"
#include "Graphics.h"
#include "Render.h"
#include "World.h"
#include "camera.h"
#include "drawFunctions.h"
#include "global.h"
#include "player.h"
#include "primitives.h"
#include "shaders.h"

void render(float delta, Graphics &graphics, Camera &camera, Gizmos &gizmos,
            WorldCtx &ctx, World &world, Player &player, Gui &gui, Input &input,
            entt::registry &colliders)
{
    glClearColor(0.0, 0.1, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -- IMGUI --

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (g.devMenu_on) {
        gui.DrawImGui(graphics, gizmos, player, camera, ctx, world, input,
                      gizmos.lines, colliders);
    } else
        gui.DrawGameInfo(camera, ctx, gizmos.lines);

    if (gizmos.points.size() > graphics.max_points * 0.8f) {
        graphics.max_points = gizmos.points.size() * 1.5f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_points * sizeof(Point),
                     nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.lines.size() > graphics.max_lines * 0.8f) {
        graphics.max_lines = gizmos.lines.size() * 1.5f;
        glBufferData(GL_ARRAY_BUFFER, graphics.max_lines * sizeof(Point),
                     nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.circles.size() > graphics.max_circles * 0.8f) {
        graphics.max_circles = gizmos.circles.size() * 1.25f;
        glBindBuffer(GL_ARRAY_BUFFER,
                     graphics.buffers[BUFF_CIRCLES]); // IMPORTANT
        glBufferData(GL_ARRAY_BUFFER, graphics.max_circles * sizeof(Shape),
                     nullptr, GL_DYNAMIC_DRAW);
    }
    if (gizmos.grass.size() > graphics.max_grass * 0.8f) {
        graphics.max_grass = gizmos.grass.size() * 1.5f;
        glBindBuffer(GL_ARRAY_BUFFER,
                     graphics.buffers[BUFF_GRASS]); // IMPORTANT
        glBufferData(GL_ARRAY_BUFFER, graphics.max_grass * sizeof(GrassBlade),
                     nullptr, GL_DYNAMIC_DRAW);
    }

    for (auto &cube : gizmos.cubes) {
        for (auto &vert : base_cube) {
            float scale = 32.0;
            vec4 color = {1.0, 0.0, 0.0, 1.0};
            Line line;
            line.start.color = color;
            line.end.color = color;
            line.start.pos = cube.pos + vert.pos * cube.size;
            line.end.pos = line.start.pos + vert.normal * scale;
            gizmos.lines.push_back(line);
        }
    }

    /*
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar *message, const void *userParam) {
            std::cout << "GL DEBUG: " << message << std::endl;
        },
                               nullptr);
     */
    // -- CAMERA -- (universal) --

    glm::mat4 view = camera.view;
    glm::mat4 proj = camera.projection;

    glBindBuffer(GL_UNIFORM_BUFFER, graphics.cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                    glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    Gfx::Use(graphics.shaders[SHADER_BASIC]);

    // -- POINTS (STATIC)--
    Gfx::UseVAO(graphics.attribs[BUFF_POINTS_STATIC]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_POINTS_STATIC]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.static_points.size() * sizeof(Point),
                    gizmos.static_points.data());
    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, gizmos.static_points.size());
    Gfx::UnbindVAO();

    // -- POINTS --
    Gfx::UseVAO(graphics.attribs[BUFF_POINTS]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_POINTS]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.points.size() * sizeof(Point),
                    gizmos.points.data());
    glPointSize(6.0f);
    glDrawArrays(GL_POINTS, 0, gizmos.points.size());
    Gfx::UnbindVAO();

    // -- LINES (STATIC) --

    Gfx::UseVAO(graphics.attribs[BUFF_LINES_STATIC]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_LINES_STATIC]);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    gizmos.static_line_points.size() * sizeof(Point),
                    gizmos.static_line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.static_line_points.size());
    Gfx::UnbindVAO();

    // -- LINES (DYNAMIC) --

    for (auto &line : gizmos.lines) {
        gizmos.line_points.push_back(line.start);
        gizmos.line_points.push_back(line.end);
    }

    Gfx::UseVAO(graphics.attribs[BUFF_LINES]);
    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_LINES]);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    gizmos.line_points.size() * sizeof(Point),
                    gizmos.line_points.data());
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gizmos.line_points.size());
    Gfx::UnbindVAO();

    // ---------------------
    // -- compressed stuff--
    // ---------------------

    Gfx::Use(graphics.shaders[SHADER_TRANSFORM]);
    Draw::Lights::DrawLights(graphics, gizmos, camera);
    player.models[0].Draw(graphics.shaders[SHADER_TRANSFORM], player.transforms[0]);

    Gfx::Use(graphics.shaders[SHADER_SIZED]);
    Draw::Lights::DrawLights(graphics, gizmos, camera);
    Draw::Shapes::DrawCubes(graphics, gizmos, camera);
    Draw::Shapes::DrawSpheres(graphics, gizmos, camera);

    // ---------------------
    // -- compressed stuff--
    // ---------------------

    // -- CIRCLES --
    Gfx::Use(graphics.shaders[SHADER_SIZED]);
    Gfx::UseVAO(graphics.attribs[BUFF_CIRCLES]);

    if (!gizmos.circles.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CIRCLES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        gizmos.circles.size() * sizeof(Shape),
                        gizmos.circles.data());
    }
    glDrawArraysInstanced(GL_LINE_LOOP, 0, base_circle.size(),
                          gizmos.circles.size());
    Gfx::UnbindVAO();

    // -- CAPSULES --
    if (g.devMenu_on) {
        Gfx::Use(graphics.shaders[SHADER_SCALE]);
        Gfx::UseVAO(graphics.attribs[BUFF_CAPSULES]);

        if (!gizmos.capsules.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CAPSULES]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            gizmos.capsules.size() * sizeof(Shape),
                            gizmos.capsules.data());
        }
        glLineWidth(1.0f);
        glDrawArraysInstanced(GL_LINES, 0, base_capsule.size(),
                              gizmos.capsules.size());
        Gfx::UnbindVAO();
    }
    // -- GROUND --

    Gfx::Use(graphics.shaders[SHADER_GROUND]);
    Gfx::UseVAO(graphics.attribs[BUFF_GROUND]);

    glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_GROUND]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gizmos.planes.size() * sizeof(Plane),
                    gizmos.planes.data());
    glDrawArraysInstanced(GL_TRIANGLES, 0, base_plane.size(),
                          gizmos.planes.size());
    Gfx::UnbindVAO();

    // -- GRASS --
    if (g.enable_grass) {
        Gfx::Use(graphics.shaders[SHADER_GRASS]);
        Gfx::UseVAO(graphics.attribs[BUFF_GRASS]);

        if (!gizmos.grass.empty()) {
            Gfx::SetFloat(graphics.shaders[SHADER_GRASS], "time", g.game_time);
            Gfx::SetVec3(graphics.shaders[SHADER_GRASS], "characterPos",
                         player.position);
            glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_GRASS]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            gizmos.grass.size() * sizeof(GrassBlade),
                            gizmos.grass.data());
        }

        glDrawArraysInstanced(GL_TRIANGLES, 0, base_triangle.size(),
                              gizmos.grass.size());

        Gfx::UnbindVAO();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    // -- DEBUG --
    /*
    Gfx::Use(graphics.shaders[SHADER_DEBUG]);
    Gfx::UseVAO(graphics.attribs[BUFF_DEBUG]);

    // update time uniform
    glBindBuffer(GL_ARRAY_BUFFER,
                 graphics.buffers[BUFF_DEBUG]);

    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    gizmos.plane.size() * sizeof(Plane),
                    gizmos.plane.data());

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_ground.size(),
                          gizmos.plane.size());
    Gfx::UnbindVAO();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL Error during render: " << err << std::endl;
    }
 */
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(g.window);
}