#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "app.h"
#include "world.h"
#include "player.h"
#include "gizmos.h"

void World::InitializeWorld(
    Player &player,
    Camera &cam,
    std::vector<Point> &points,
    std::vector<Point> &lines,
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
  points.push_back(player.pos_dot);
  points.push_back(player.xhair_dot);

  InitWorld(lines);

  cells.reserve(grid_width * grid_width);
  for (int y = 0; y < grid_width; ++y)
  {
    for (int x = 0; x < grid_width; ++x)
    {
      Cell cell;
      cell.id = y * grid_width + x;

      glm::vec2 start = origin - (grid_width * grid_square_size) / 2.0f;
      cell.pos = start + glm::vec2(x + 0.5f, y + 0.5f) * grid_square_size;

      cells.push_back(cell);
    }
  }
  House house;
  house.id = 1;
  house.cell = 1820;
  structures.push_back(house);
/* 
  glBindBuffer(GL_ARRAY_BUFFER, vbo_points);
  glBufferData(GL_ARRAY_BUFFER,
               points.size() * sizeof(Point),
               points.data(),
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_lines);
  glBufferData(GL_ARRAY_BUFFER,
               lines.size() * sizeof(Point),
               lines.data(),
               GL_DYNAMIC_DRAW); */
}

void World::InitWorld(std::vector<Point> &lines)
{
  InitGrid(lines, glm::vec2(0.0f, 0.0f), grid_width, grid_square_size);

  InitCompas(lines);
}
void DrawCube(glm::vec3 center, float size)
{
  // re implement
}

void World::DrawHouse()
{
  for (auto &cell : cells)
    if (cell.id == 1820)
    {
      for (auto &house : structures)
        DrawCube(glm::vec3(cell.pos.x, cell.pos.y, 64.0f), 64.0f);
    }
}
void World::InitCompas(std::vector<Point> &lines)
{
  for (auto line : compas.lines)
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

void World::EraseBlasts(std::vector<Point> &lines)
{
  blasts.erase(
      std::remove_if(blasts.begin(), blasts.end(),
                     [&](Blast &b)
                     {
                       if (b.cooldown <= 0.0f)
                       {
                         // Remove vertices from the global buffer
                         if (b.vertex_start + b.vertex_amount <= lines.size())
                           lines.erase(lines.begin() + b.vertex_start,
                                       lines.begin() + b.vertex_start + b.vertex_amount);

                         // Update remaining blasts
                         size_t removed_count = b.vertex_amount;
                         for (auto &other : blasts)
                           if (other.vertex_start > b.vertex_start)
                             other.vertex_start -= removed_count;

                         return true;
                       }
                       return false;
                     }),
      blasts.end());
}

void BlastManager::CreateBlast(float size, float rate, glm::vec3 pos, std::vector<Blast> &blasts, std::vector<Circle> &circles)
{
  std::cout << "Creating blast" << std::endl;
  //int segments = 16;
  //glm::vec3 color = {1.0f, 0.0f, 0.0f};

  //Circle circle = CreateCircle(size, color, pos, circles);
  Blast blast{size, rate, rate, pos};
  blasts.push_back(blast);
  //std::cout << "Created blast" << std::endl;
}

void BlastManager::UpdateBlasts(double time_elapsed, std::vector<Blast> &blasts, std::vector<Circle> &circles)
{
  glm::vec3 color = {1.0f,0.0f,0.0f};

  for (auto &blast : blasts)
  {
    if (blast.cooldown > 0.0f)
    {
      blast.cooldown -= time_elapsed;

      blast.size = blast.max_size * (1.0f - float(blast.cooldown / blast.rate));

      Circle circle{ // color, center, size
        color,
        blast.pos,
        blast.size
      };
      circles.push_back(circle);

      if (blast.cooldown < 0.0f)
        blast.cooldown = 0.0f;
    }
    else
      blast.cooldown = 0.0;
  }
}

void Bullet::DrawBullet(double delta)
{

  if (time > 0.0f)
  {
    time -= delta;
    if (time < 0.0f)
      time = 0.0f;
  }
  /* re implement
    float lineWidth = 1.0f;
    glLineWidth(lineWidth);
    glColor3f(1.0f, 0.2f, 0.2f);

    glBegin(GL_LINES);
    glVertex2f(start_pos_x, start_pos_y);
    glVertex2f(pos_x, pos_y);
    glEnd(); */
}

void World::EraseBullets()
{
  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](const Bullet &b)
                     { return b.time <= 0.0f; }),
      bullets.end());
}
