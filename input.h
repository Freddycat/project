#ifndef INPUT_H
#define INPUT_H

#include <vector>

struct App;
struct Dot;
struct Camera;

struct Input
{
  float m_screen_pos_x;
  float m_screen_pos_y;

  float m_world_pos_x;
  float m_world_pos_y;

  void getMouseInput(App &app, Camera &cam);

  void getMouseWorldPos(App &app, Camera &cam);

  void inputKeyboard(Dot &dot);
};

#endif
