#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "app.h"
#include "world.h"
#include "player.h"

void World::initializeWorld()
{
  Weapon deagle;
  Weapon blast;

  blast.type = WeaponType::Blast;
  deagle.type = WeaponType::Deagle;

  app_ptr->player.pos_x = 0.0f;
  app_ptr->player.pos_y = 0.0f;

  app_ptr->player.weapons.push_back(blast);
  app_ptr->player.weapons.push_back(deagle);
}

void World::drawWorld()
{
  drawGrid(grid_pos_x,
           grid_pos_y,
           (int)grid_squares,
           (int)grid_squares,
           grid_square_size);

  drawCompas();
}

void World::drawCompas()
{
  float lineWidth = 2.0f;
  glLineWidth(lineWidth);

  // X axis - red
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(100.0f, 0.0f, 0.0f);
  glEnd();

  // Y axis - green
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 100.0f, 0.0f);
  glEnd();

  // Z axis - blue
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 100.0f);
  glEnd();
}

void drawCell(float x, float y, float cellSize)
{
  glBegin(GL_QUADS);
  glVertex2f(x, y);
  glVertex2f(x + cellSize, y);
  glVertex2f(x + cellSize, y + cellSize);
  glVertex2f(x, y + cellSize);
  glEnd();
}

// draw filled grid cells (row/col), origin at top-left or world origin
void drawGrid(float originX, float originY,
              int cols, int rows,
              float cellSize)
{
  float lineWidth = 1.0f;
  glLineWidth(lineWidth);
  glColor3f(0.2f, 0.2f, 0.2f);

  // draw vertical lines
  glBegin(GL_LINES);
  for (int c = 0; c <= cols; ++c)
  {
    float x = originX + c * cellSize;
    float y0 = originY;
    float y1 = originY + rows * cellSize;
    glVertex2f(x, y0);
    glVertex2f(x, y1);
  }
  // draw horizontal lines
  for (int r = 0; r <= rows; ++r)
  {
    float y = originY + r * cellSize;
    float x0 = originX;
    float x1 = originX + cols * cellSize;
    glVertex2f(x0, y);
    glVertex2f(x1, y);
  }
  glEnd();
  // drawCell(originX, originY, cellSize);
}

void World::eraseBlasts()
{
  blasts.erase(
      std::remove_if(blasts.begin(), blasts.end(),
                     [](const Blast &b)
                     { return b.time <= 0.0f; }),
      blasts.end());
}

void World::eraseBullets()
{
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](const Bullet &b)
                     { return b.time <= 0.0f; }),
      bullets.end());
}

void Blast::drawBlast(double delta)
{
  //std::cout << "Drawing blast" << std::endl;
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

void Bullet::drawBullet(double delta)
{

  if (time > 0.0f)
  {
    time -= delta;
    if (time < 0.0f)
      time = 0.0f;
  }

  float lineWidth = 1.0f;
  glLineWidth(lineWidth);
  glColor3f(1.0f, 0.2f, 0.2f);

  glBegin(GL_LINES);
  glVertex2f(start_pos_x, start_pos_y);
  glVertex2f(pos_x, pos_y);
  glEnd();
}