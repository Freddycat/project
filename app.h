#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>

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

  int window_width;
  int window_height;

  float window_center_x;
  float window_center_y;

  double game_time;

  int target_framerate = 60;

  double delta;

  std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));

  std::chrono::steady_clock::time_point frame_last;
  std::chrono::steady_clock::time_point frame_now;
  
  std::chrono::steady_clock::time_point next_frametime;

  void checkSize()
  {
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Center coordinates
    window_center_x = window_width / 2.0f;
    window_center_y = window_height / 2.0f;
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