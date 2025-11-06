#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <string>

#include "input.h"
#include "gui.h"
#include "camera.h"
#include "world.h"
#include "player.h"

extern SDL_Window *window;

struct App
{

  Input input;
  Camera cam;
  Gui gui;
  Player player;
  World world;

  bool running = false;

  glm::vec2 window_size;
  int window_width;
  int window_height;

  glm::vec2 window_center;
  //float window_center_x;
  //float window_center_y;

  double game_time = 0;

  int target_framerate = 60;

  double delta;

  std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));

  std::chrono::steady_clock::time_point frame_last;
  std::chrono::steady_clock::time_point frame_now;

  std::chrono::steady_clock::time_point next_frametime;

  std::string formatTime() const;

  void checkSize()
  {
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Center coordinates
    window_center.x = window_width / 2.0f;
    window_center.y = window_height / 2.0f;

    window_center.x = window_width / 2.0f;
    window_center.y = window_height / 2.0f;

    cam.window_center = window_center;
    input.window_center = window_center;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    cam.projection = glm::ortho(
        0.0f, 800.0f,     // left, right
        600.0f, 0.0f,     // bottom, top
        -1000.0f, 2000.0f // near, far
    );

    glOrtho(0, 800, 600, 0, -1000, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_POINT_SMOOTH);
  }

  App()
  {
    input.setApp(*this);
    cam.setApp(*this);
    gui.setApp(*this);
    world.setApp(*this);
    player.setApp(*this);
  }
};

#endif