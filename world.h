#ifndef WORLD_H
#define WORLD_H

#include <vector>

struct App;
struct Blast;
struct Bullet;
struct Player;
struct Camera;

struct World
{
  float grid_pos_x = -800.0f;
  float grid_pos_y = -600.0f;
  int grid_squares = 64;
  float grid_square_size = 64.0f;

  std::vector<Blast> blasts;
  std::vector<Bullet> bullets;

  void InitializeWorld(Player &player, Camera &cam);
  void DrawWorld();
  void DrawCompas();
  void EraseBlasts();
  void EraseBullets();
};

void DrawGrid(float originX, float originY,
              int cols, int rows,
              float cellSize);

struct Blast
{
  float radius;
  double rate;
  double time;
  float pos_x;
  float pos_y;
  void DrawBlast(double delta);
};

struct Bullet
{
  float size;
  double time;
  float start_pos_x;
  float start_pos_y;
  float pos_x;
  float pos_y;
  void DrawBullet(double delta);
};

#endif