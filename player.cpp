#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include "player.h"
#include "app.h"

void Player::drawPlayer()
{
  glColor3f(0.6f, 0.0f, 0.6f);
  glPointSize(8.0f);
  glBegin(GL_POINTS);
  glVertex2f(posX, posY);
  glEnd();
}

void Player::drawCrosshair()
{

  if (!app_ptr)
    return;

  glColor3f(1.0f, 0.0f, 0.0f);
  glPointSize(4.0f);
  glBegin(GL_POINTS);
  glVertex2f(app_ptr->input.m_world_pos_x, app_ptr->input.m_world_pos_y);
  glEnd();
}

void Player::drawBlast()
{
  
  if (!app_ptr)
    return;
  
  int segments = 32;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_LOOP);

  for (int i = 0; i < segments; i++)
  {
    float theta = 2.0f * 3.1415926f * float(i) / float(segments);
    float x = weapon.blast_size * 10.0f * cosf(theta);
    float y = weapon.blast_size * 10.0f * sinf(theta);
    glVertex2f(app_ptr->input.m_world_pos_x + x,
               app_ptr->input.m_world_pos_y + y);
  }

  glEnd();
}