#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "app.h"
#include "world.h"
#include "player.h"
#include "gizmos.h"

World world;

void World::InitializeWorld(
    Player &player,
    Camera &cam,
    Gizmos &gizmos,
    GLuint &shaderID,
    GLuint &vbo_points,
    GLuint &vbo_lines)
{
  cam.SetCam(shaderID);

  Weapon deagle;
  Weapon blast;

  blast.type = WeaponType::Blast;
  deagle.type = WeaponType::Deagle;

  // player
  player.pos_x = 0.0f;
  player.pos_y = 0.0f;
  player.weapons.push_back(blast);
  player.weapons.push_back(deagle);
  gizmos.points.push_back(player.pos_dot);
  gizmos.points.push_back(player.xhair_dot);
  gizmos.capsules.push_back(player.playerCap);

  cells.reserve(grid_width * grid_width);
  for (int y = 0; y < grid_width; ++y)
  {
    for (int x = 0; x < grid_width; ++x)
    {
      Cell cell;
      cell.id = y * grid_width + x;

      glm::vec3 start = origin;
      start.x -= (grid_width * grid_square_size) / 2.0f;
      start.y -= (grid_width * grid_square_size) / 2.0f;
      cell.pos = start + glm::vec3(x + 0.5f, y + 0.5f, 0.0f) * grid_square_size;

      cells.push_back(cell);
    }
  }
  std::cout << "cells.size(): " << cells.size() << std::endl;

  House house;
  house.id = 1;
  house.cell = 1820;
  structures.push_back(house);

  InitWorld(gizmos);

  std::cout << "Initialized world." << std::endl;
}

void World::InitWorld(Gizmos &gizmos)
{
  InitGrid(gizmos.static_line_points, glm::vec2(0.0f, 0.0f), grid_width, grid_square_size);

  InitCompas(gizmos.static_line_points);

  World::InitCube(gizmos.cubes);
}

void World::InitCube(std::vector<Cube> &cubes)
{
  glm::vec3 color = {0.0f, 1.0f, 1.0f};
  float size = 100.0f;

  std::cout << "trying to find cell" << std::endl;

  for (auto &cell : cells)
    if (cell.id == 1820)
    {
      glm::vec3 center = cell.pos;
      center.z += size / 2;
      std::cout << "found cell" << center.x << center.y << center.z << std::endl;
      for (auto &house : structures)
      {
        std::cout << "found house" << std::endl;
        Cube cube = CreateCube(center, size, color, cubes);
      }
    }
}
void World::InitCompas(std::vector<Point> &lines)
{
  for (auto line : compass.lines)
  {
    Point a, b;
    a.pos = line.start;
    a.color = line.color;

    b.pos = line.end;
    b.color = line.color;
    lines.push_back(a);
    lines.push_back(b);
  }
}

void DrawCell(float x, float y, float cellSize)
{ /* re implement
   glBegin(GL_QUADS);
   glVertex2f(x, y);
   glVertex2f(x + cellSize, y);
   glVertex2f(x + cellSize, y + cellSize);
   glVertex2f(x, y + cellSize);
   glEnd(); */
}

void InitGrid(std::vector<Point> &lines, glm::vec2 origin, int width, float cellSize)
{

  glm::vec2 start = origin - (width * cellSize) / 2.0f;
  glLineWidth(1.0f);
  glm::vec3 color = {0.2f, 0.2f, 0.2f};

  for (int amount = 0; amount <= width; ++amount)
  {
    float x = start.x + amount * cellSize;
    float y_start = start.y;
    float y_end = start.y + width * cellSize;
    Point a, b;
    a.color = color;
    b.color = color;
    a.pos = glm::vec3(x, y_start, 0.0);
    b.pos = glm::vec3(x, y_end, 0.0);
    lines.push_back(a);
    lines.push_back(b);
  }
  // draw horizontal lines
  for (int amount = 0; amount <= width; ++amount)
  {
    float y = start.y + amount * cellSize;
    float x_start = start.x;
    float x_end = start.x + width * cellSize;
    Point a, b;
    a.color = color;
    b.color = color;
    a.pos = glm::vec3(x_start, y, 0.0);
    b.pos = glm::vec3(x_end, y, 0.0);
    lines.push_back(a);
    lines.push_back(b);
  }
}

void CreateBullet(glm::vec3 player_pos, glm::vec3 mouse_pos, double cooldown, std::vector<Bullet> &bullets)
{
  Bullet bullet{
      player_pos,
      mouse_pos,
      cooldown};
  bullets.push_back(bullet);
}

void BlastManager::CreateBlast(float size, float rate, glm::vec3 pos, std::vector<Blast> &blasts)
{
  // std::cout << "Creating blast" << std::endl;
  // int segments = 16;
  // glm::vec3 color = {1.0f, 0.0f, 0.0f};

  // Circle circle = CreateCircle(size, color, pos, circles);
  Blast blast{size, rate, rate, pos};
  blasts.push_back(blast);
  // std::cout << "Created blast" << std::endl;
}

void BlastManager::UpdateBlasts(double time_elapsed, std::vector<Blast> &blasts, std::vector<Circle> &circles)
{
  glm::vec3 color = {1.0f, 0.0f, 0.0f};

  for (auto &blast : blasts)
  {
    if (blast.cooldown > 0.0f)
    {
      blast.cooldown -= time_elapsed;

      blast.size = blast.max_size * (1.0f - float(blast.cooldown / blast.rate));

      Circle circle{// color, center, size
                    blast.pos,
                    blast.size,
                    color};
      circles.push_back(circle);

      if (blast.cooldown < 0.0f)
        blast.cooldown = 0.0f;
    }
    else
      blast.cooldown = 0.0;
  }
}

void UpdateBullets(double time_elapsed, std::vector<Bullet> &bullets, std::vector<Line> &lines)
{
  glm::vec3 color = {1.0f, 0.0f, 0.0f};

  for (auto &bullet : bullets)
  {
    if (bullet.cooldown > 0.0f)
    {
      bullet.cooldown -= time_elapsed;

      Line line{

          bullet.pos_start,
          bullet.pos_end,
          color
      };

      lines.push_back(line);

      if (bullet.cooldown < 0.0f)
        bullet.cooldown = 0.0f;
    }
    else
      bullet.cooldown = 0.0;
  }
}

/*
void Bullet::DrawBullet(double delta)
{

  if (time > 0.0f)
  {
    time -= delta;
    if (time < 0.0f)
      time = 0.0f;
  }
} */

void World::EraseBullets()
{
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](const Bullet &b)
                     { return b.cooldown <= 0.0f; }),
      bullets.end());
}
