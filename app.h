#ifndef APP_H
#define APP_H

#include <glad/glad.h>

struct Dot
{
  float posX = 400.0f;
  float posY = 280.0f;
  float speed = 6.0f;

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
  }

  void draw()
  {
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(posX, posY);
    glEnd();
  }
};

#endif