#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "app.h"
#include "app_start.h"
#include "shaders.h"
#include "render.h"
#include "global.h"

Global g;

SDL_Window *window = nullptr;
SDL_GLContext glContext;

bool running = true;

std::string message = "so this is the message";
int number = 42;
int frameNumber = 0;

int target_framerate = 60;
std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));
std::chrono::steady_clock::time_point frame_last;
std::chrono::steady_clock::time_point frame_now;
std::chrono::steady_clock::time_point next_frametime;

void gameLoop(SDL_Event &event)
{
  // -- start time stuff --
  Input *input = Input::Instance();

  frame_now = std::chrono::steady_clock::now();
  g.time_elapsed = std::chrono::duration<double>(frame_now - frame_last).count();
  frame_last = frame_now;
  g.game_time += g.time_elapsed;

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
    {
      running = false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
    {
      g.console_on = !g.console_on;
    }
  }

  // -- CLEARING (DYNAMIC)--

  gizmos.lines.clear();
  gizmos.line_points.clear();
  gizmos.circles.clear();

  // -- UPDATING --

  input->InputKeyboard();
  input->GetMouseInput();
  player.MovePlayer();
  camera.CenterCam(graphics.vertexID);
  input->GetMouseWorldPos();

  player.UpdateCrosshair(gizmos.points, input->mouse.world_pos);
  player.UpdatePlayerDot(gizmos.points, gizmos.capsules);

  for (auto &weapon : player.weapons)
    weapon.UpdateWeapon(*input, g.time_elapsed, weapon.type);

  BlastManager::UpdateBlasts(g.time_elapsed, world.blasts, gizmos.circles);
  UpdateBullets(g.time_elapsed, world.bullets, gizmos.lines);

  // -- end updating --
  // -- start render --
  render();

  // world.EraseBlasts(gizmos.line_points_static);
  // world.EraseBullets();

  frameNumber++;

  next_frametime = target_frametime + frame_now;
  std::this_thread::sleep_until(next_frametime);
}

void gameStart()
{
  std::cout << message << std::endl;
  std::cout << number << std::endl;

  if (!initializeSDL())
  {
    std::cerr << "SDL initialization failed. Exiting." << std::endl;
    return;
  }

  std::cout << "SDL initialized." << std::endl;

  if (!initializeGL())
  {
    std::cerr << "GL initialization failed. Exiting." << std::endl;
    return;
  }

  std::cout << "GL initialized." << std::endl;

  initializeImgui();

  std::cout << "ImGui initialized." << std::endl;

  CheckSize();

  std::cout << "Checked size." << std::endl;

  SetupShaders();

  world.InitializeWorld(
      player, camera, gizmos,
      graphics.vertexID,
      graphics.vbo_point,
      graphics.vbo_line);

  frame_last = std::chrono::steady_clock::now();
  frame_now = frame_last;
  g.game_time = 0.0;
  g.time_elapsed = 0.0;

  running = true;
  g.console_on = true;

  SDL_Event event;
  while (running)
  {
    gameLoop(event);
  }
}

void CheckSize()
{
  SDL_GetWindowSize(g.window, &g.window_width, &g.window_height);

  // Center coordinates
  g.window_center.x = g.window_width / 2.0f;
  g.window_center.y = g.window_height / 2.0f;
}

std::string formatTime()
{
  int total_ms = (int)std::floor(g.game_time * 1000.0);
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