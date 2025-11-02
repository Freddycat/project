#ifndef INPUT_H
#define INPUT_H

#include <vector>

struct App;
struct Player;
struct Camera;

struct Input
{

  App *app_ptr = nullptr;
  float m_screen_pos_x;
  float m_screen_pos_y;

  float m_world_pos_x;
  float m_world_pos_y;

  void getMouseInput();

  void getMouseWorldPos();

  void inputKeyboard(Player &player);

  void setApp(App &app)
  {
    app_ptr = &app;
  }

};

#endif
