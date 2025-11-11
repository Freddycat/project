#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "app.h"
#include "app_start.h"

App app;

SDL_Window *window = nullptr;
SDL_GLContext glContext;

auto &time_elapsed = app.state.time;
auto &game_time = app.state.game_time;
auto &state = app.state;

bool imgui_on = true;

std::string message = "so this is the message";
int number = 42;
int frameNumber = 0;

void gameLoop(SDL_Event &event)
{
  app.frame_now = std::chrono::steady_clock::now();

  time_elapsed = std::chrono::duration<double>(app.frame_now - app.frame_last).count();

  app.frame_last = app.frame_now;

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
    {
      app.running = false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_X)
    {
      imgui_on = !imgui_on;
    }
  }

  app.input.InputKeyboard(app.player);

  app.input.GetMouseInput();
  app.player.MovePlayer();
  app.camera.CenterCam(app.graphics.vertexID, app.input, app.player);
  app.input.GetMouseWorldPos(state, app.camera);

  game_time += time_elapsed;
  std::string time_str = app.formatTime();

  // -- end updating --
  // -- start render --

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  if (imgui_on)
  {

    app.gui.DrawWindow(state, app.graphics, app.gizmos, app.input, app.camera, app.world, app.player, time_str);

    app.gui.DrawList(state, app.input, app.world, app.camera);
  }
  // app.world.DrawWorld(app.lines);

  // app.world.DrawHouse();

  // clear circles
  app.gizmos.circles.clear();

  app.player.DrawCrosshair(app.graphics.points, app.input.mouse_world_pos);
  app.player.DrawPlayer(app.graphics.points);

  for (auto &weapon : app.player.weapons)
    weapon.UpdateWeapon(app.input, app.world, app.player, time_elapsed, weapon.type, app.graphics.lines, app.gizmos.circles);

  BlastManager::UpdateBlasts(time_elapsed, app.world.blasts, app.gizmos.circles);
  UpdateBullets(time_elapsed, app.world.bullets, app.graphics.lines);
  
/* 
  for (auto &bullet : app.world.bullets)
    bullet.DrawBullet(time_elapsed);
 */
  glUseProgram(app.graphics.vertexID);

  if (app.graphics.points.size() > app.graphics.max_points)
  {
    app.graphics.max_points = app.graphics.points.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_points * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
  }

  if (app.graphics.lines.size() > app.graphics.max_lines)
  {
    app.graphics.max_lines = app.graphics.lines.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_lines * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
  }

  if (app.gizmos.circles.size() > app.graphics.max_circles)
  {
    app.graphics.max_circles = app.gizmos.circles.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_circles * sizeof(Circle), nullptr, GL_DYNAMIC_DRAW);
  }

  GLuint projLocation = glGetUniformLocation(app.graphics.vertexID, "uProjection");
  GLuint viewLocation = glGetUniformLocation(app.graphics.vertexID, "uView");
  glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(app.camera.projection));
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  glBindVertexArray(app.graphics.vao_point);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_point);

  glBufferSubData(GL_ARRAY_BUFFER, 0, app.graphics.points.size() * sizeof(Point), app.graphics.points.data());
  glPointSize(6.0f);

  glDrawArrays(GL_POINTS, 0, app.graphics.points.size());

  glBindVertexArray(app.graphics.vao_line);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_line);

  glBufferSubData(GL_ARRAY_BUFFER, 0, app.graphics.lines.size() * sizeof(Point), app.graphics.lines.data());
  glLineWidth(2.0f);

  glDrawArrays(GL_LINES, 0, app.graphics.lines.size());

  glBindVertexArray(0);

  // circle render
  glUseProgram(app.graphics.circleID);
  // camera stuff
  GLuint projLocationCirc = glGetUniformLocation(app.graphics.circleID, "uProjection");
  GLuint viewLocationCirc = glGetUniformLocation(app.graphics.circleID, "uView");
  glUniformMatrix4fv(projLocationCirc, 1, GL_FALSE, glm::value_ptr(app.camera.projection));
  glUniformMatrix4fv(viewLocationCirc, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  glBindVertexArray(app.graphics.vao_circle);

  if (!app.gizmos.circles.empty())
  {
    glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_circles);

    glBufferSubData(GL_ARRAY_BUFFER, 0, app.gizmos.circles.size() * sizeof(Circle), app.gizmos.circles.data());
  }

  glDrawArraysInstanced(GL_LINE_LOOP, 0, app.graphics.base_circle.size(), app.gizmos.circles.size());

  glBindVertexArray(0);

  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
    std::cerr << "GL error: " << err << std::endl;

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);

  app.world.EraseBlasts(app.graphics.lines);
  app.world.EraseBullets();

  frameNumber++;

  app.next_frametime = app.target_frametime + app.frame_now;
  std::this_thread::sleep_until(app.next_frametime);
}

void gameStart()
{
  std::cout << message << std::endl;
  std::cout << number << std::endl;

  if (!initializeSDL(app))
  {
    std::cerr << "SDL initialization failed. Exiting." << std::endl;
    return;
  }

  // SDL_SetWindowRelativeMouseMode(window, true);

  std::cout << "SDL initialized." << std::endl;

  if (!initializeGL(app, glContext, window))
  {
    std::cerr << "GL initialization failed. Exiting." << std::endl;
    return;
  }

  std::cout << "GL initialized." << std::endl;

  initializeImgui(glContext);

  std::cout << "ImGui initialized." << std::endl;

  app.CheckSize();

  std::cout << "Checked size." << std::endl;

  app.world.InitializeWorld(
      app.player, app.camera,
      app.graphics.points, app.graphics.lines,
      app.graphics.vertexID,
      app.graphics.vbo_point,
      app.graphics.vbo_line);

  std::cout << "Initialized world." << std::endl;

  app.frame_last = std::chrono::steady_clock::now();
  app.frame_now = app.frame_last;
  game_time = 0.0;
  time_elapsed = 0.0;

  app.state.running = true;

  SDL_Event event;
  while (app.state.running)
  {
    gameLoop(event);
  }
}

void App::CheckSize()
{
  SDL_GetWindowSize(window, &state.window_width, &state.window_height);

  // Center coordinates
  state.window_center.x = state.window_width / 2.0f;
  state.window_center.y = state.window_height / 2.0f;

  camera.window_center = state.window_center;
  input.window_center = state.window_center;
}

std::string App::formatTime() const
{
  int total_ms = (int)std::floor(game_time * 1000.0);
  int ms = total_ms % 1000;
  int total_seconds = total_ms / 1000;
  int s = total_seconds % 60;
  int total_minutes = total_seconds / 60;
  int m = total_minutes % 60;
  int h = total_minutes / 60;
  char buf[64];
  std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d", h, m, s, ms);
  // std::cout << "time done:" << buf << std::endl;
  return std::string(buf);
}

int main()
{
  gameStart();
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}