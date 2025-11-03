#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <glm/glm.hpp>

struct App;
struct Player;
struct Camera;

struct Input
{

  App *app_ptr = nullptr;

  glm::vec2 mouse_screen_pos;
  float mouse_screen_pos_x;
  float mouse_screen_pos_y;

  float mouse_to_center_pos_x;
  float mouse_to_center_pos_y;

  glm::vec2 mouse_world_pos;
  float mouse_world_pos_x;
  float mouse_world_pos_y;

  void getMouseInput();

  void getMouseWorldPos();

  void inputKeyboard(Player &player);

  void setApp(App &app)
  {
    app_ptr = &app;
  }

};

#endif
