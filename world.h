#ifndef WORLD_H
#define WORLD_H

#include <vector>

struct App;
struct Blast;

struct World
{
  App *app_ptr = nullptr;

  float grid_pos_x = -800.0f;
  float grid_pos_y = -600.0f;
  int grid_squares = 64;
  float grid_square_size = 64.0f;

  std::vector<Blast> blasts;

  void initializeWorld();
  void drawWorld();
  void eraseBlasts();

  void setApp(App &app)
  {
    app_ptr = &app;
  };
};

void drawGrid(float originX, float originY,
              int cols, int rows,
              float cellSize);

struct Blast
{
  float radius;
  double rate;
  double time;
  float pos_x;
  float pos_y;
  void drawBlast(double delta);
};

#endif