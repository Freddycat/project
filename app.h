#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>

extern SDL_Window *window;

struct App
{

  int window_width;
  int window_height;

  float window_center_x;
  float window_center_y;

  float cam_point_x;
  float cam_point_y;

  void checkSize()
  {
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Center coordinates
    window_center_x = window_width / 2.0f;
    window_center_y = window_height / 2.0f;
  }
};

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

  /*
    void checkEdge()
    {

      if (posX >= 800.0f)
        posX = 1.0f;
      if (posX <= 0.0f)
        posX = 799.0f;
      if (posY >= 600.0f)
        posY = 1.0f;
      if (posY <= 0.0f)
        posY = 599.0f;
    } */

  void drawDot()
  {
    glColor3f(0.5f, 0.0f, 0.5f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(posX, posY);
    glEnd();
  }
};

#endif