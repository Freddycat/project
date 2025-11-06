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
#include "app_state.h"

extern SDL_Window *window;

struct App
{
  Input input;
  Camera cam;
  Gui gui;
  Player player;
  World world;
  State state;

  bool running = false;

  int target_framerate = 60;
  
  std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));

  std::chrono::steady_clock::time_point frame_last;
  std::chrono::steady_clock::time_point frame_now;

  std::chrono::steady_clock::time_point next_frametime;

  std::string formatTime() const;

  void CheckSize();
  /*
  App()
  {
    input.setApp(*this);
    cam.setApp(*this);
    gui.setApp(*this);
    world.setApp(*this);
    player.setApp(*this);
  }
  */
};

#endif