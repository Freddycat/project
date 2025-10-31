#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "input.h"
#include "gui.h"
#include "camera.h"

extern SDL_Window *window;
struct Input;
struct Camera;

struct Dot
{
  float posX = 0.0f;
  float posY = 0.0f;
  float speed = 6.0f;

  struct Weapon
  {
    float blast_size = 15.0f;
    float blast_speed = 6.0f;
    float rate = 9.0f;
  } weapon;

  void drawDot()
  {
    glColor3f(0.6f, 0.0f, 0.6f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(posX, posY);
    glEnd();
  }
};

struct App
{

  Input input;
  Camera cam;
  Gui gui;

  bool running = false;

  int window_width;
  int window_height;

  float window_center_x;
  float window_center_y;

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
  }
};

#endif