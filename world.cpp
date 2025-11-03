#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "app.h"
#include "world.h"
#include "player.h"

void World::initializeWorld()
{
  Weapon blast;
  app_ptr->player.pos_x = 0.0f;
  app_ptr->player.pos_y = 0.0f;
  app_ptr->player.weapons.push_back(blast);
}

void World::drawWorld()
{
  glColor3f(0.8f, 0.2f, 0.2f); // red (bottom-right)
  glBegin(GL_QUADS);
  glVertex2f(0.0f, 0.0f);     // Top-left
  glVertex2f(800.0f, 0.0f);   // Top-right
  glVertex2f(800.0f, 600.0f); // Bottom-right
  glVertex2f(0.0f, 600.0f);   // Bottom-left
  glEnd();

  glColor3f(0.2f, 0.8f, 0.2f); // green (bottom-left)
  glBegin(GL_QUADS);
  glVertex2f(-800.0f, 0.0f);   // Top-left
  glVertex2f(0.0f, 0.0f);      // Top-right
  glVertex2f(0.0f, 600.0f);    // Bottom-right
  glVertex2f(-800.0f, 600.0f); // Bottom-left
  glEnd();

  glColor3f(0.2f, 0.2f, 0.8f); // blue (top-left)
  glBegin(GL_QUADS);
  glVertex2f(-800.0f, -600.0f); // Top-left
  glVertex2f(0.0f, -600.0f);    // Top-right
  glVertex2f(0.0f, 0.0f);       // Bottom-right
  glVertex2f(-800.0f, 0.0f);    // Bottom-left
  glEnd();

  glColor3f(0.8f, 0.8f, 0.8f); // white (top right)
  glBegin(GL_QUADS);
  glVertex2f(0.0f, -600.0f);   // Top-left
  glVertex2f(800.0f, -600.0f); // Top-right
  glVertex2f(800.0f, 0.0f);    // Bottom-right
  glVertex2f(0.0f, 0.0f);      // Bottom-left
  glEnd();
}

void World::eraseBlasts()
{
  blasts.erase(
      std::remove_if(blasts.begin(), blasts.end(),
                     [](const Blast &b)
                     { return b.time <= 0.0f; }),
      blasts.end());
}

void Blast::drawBlast(double delta)
{
  std::cout << "Drawing blast" << std::endl;
  int segments = 16;
  float size = 0.0f;
  if (time > 0.0f)
  {
    size = radius * (1.0f - float(time / rate));
    time -= delta;
    if (time < 0.0f)
      time = 0.0f;
  }
  else
    time = 0.0;

  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_LOOP);

  for (int i = 0; i < segments; i++)
  {
    float theta = 2.0f * 3.1415926f * float(i) / float(segments);
    float x = size * 10.0f * cosf(theta);
    float y = size * 10.0f * sinf(theta);
    glVertex2f(pos_x + x,
               pos_y + y);
  }

  glEnd();
}