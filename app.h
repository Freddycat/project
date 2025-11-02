#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>

#include "input.h"
#include "gui.h"
#include "camera.h"
#include "player.h"

extern SDL_Window *window;

struct App
{

  Input input;
  Camera cam;
  Gui gui;
  Player player;

  bool running = false;

  int window_width;
  int window_height;

  float window_center_x;
  float window_center_y;

  std::chrono::duration<double> game_time;

  int target_framerate = 60;

  std::chrono::duration<double> delta;

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
    player.setApp(*this);
  }
};

#endif