#ifndef WORLD_H
#define WORLD_H

#include <vector>

struct App;
struct Blast;

struct World
{
  App *app_ptr = nullptr;

  std::vector<Blast> blasts;

  void initializeWorld();
  void drawWorld();
  void eraseBlasts();

  void setApp(App &app)
  {
    app_ptr = &app;
  };
};


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