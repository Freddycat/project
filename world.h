#ifndef WORLD_H
#define WORLD_H

#include <vector>

struct App;
struct Blast;
struct Bullet;
struct Player;
struct Camera;
struct Cell;
struct House;

struct World
{
  glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);

  int grid_width = 64;
  float grid_square_size = 64.0f;

  std::vector<Blast> blasts;
  std::vector<Bullet> bullets;
  std::vector<Cell> cells;
  std::vector<House> structures;

  struct Compass
  {
    glm::vec3 color[3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}};

    std::vector<Line> lines = {
        // start, end, color
        {{0, 0, 0}, {100, 0, 0}, color[0]}, // X axis
        {{0, 0, 0}, {0, 100, 0}, color[1]}, // Y axis
        {{0, 0, 0}, {0, 0, 100}, color[2]}  // Z axis
    };
  } compass;

  void InitializeWorld(
      Player &player,
      Camera &cam,
      Gizmos &gizmos,
      GLuint &shaderID,
      GLuint &vbo_point,
      GLuint &vbo_line);

  void InitWorld(Gizmos &gizmos);
  void InitCompas(std::vector<Point> &lines);
  void InitCube(std::vector<Cube> &cubes);
  void EraseBlasts(std::vector<Point> &lines);
  void EraseBullets();
};

extern World world;

void InitGrid(std::vector<Point> &lines, glm::vec2 origin, int amount, float cellSize);

struct Cell
{
  int id;
  glm::vec3 pos;
};

struct House
{
  int id;
  glm::vec2 pos;
  int cell;
};

namespace BlastManager
{
  void CreateBlast(float size, float rate, glm::vec3 pos, std::vector<Blast> &blasts);
  void UpdateBlasts(double time_elapsed, std::vector<Blast> &blasts, std::vector<Circle> &circles);
}

void CreateBullet(glm::vec3 player_pos, glm::vec3 mouse_pos, double cooldown, std::vector<Bullet> &bullets);
void UpdateBullets(double time_elapsed, std::vector<Bullet> &bullets, std::vector<Line> &lines);

struct Blast
{
  float max_size;
  double rate;
  double cooldown;
  glm::vec3 pos;
  float size;
  size_t vertex_start;
  size_t vertex_amount;
};

struct Bullet
{
  glm::vec3 pos_start;
  glm::vec3 pos_end;
  double cooldown;

  void DrawBullet(double delta);
};

#endif