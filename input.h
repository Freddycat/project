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

  glm::vec2 window_center;

  glm::vec2 mouse_screen_pos;

  glm::vec2 mouse_center_pos;

  //float mouse_screen_pos_x;
  //float mouse_screen_pos_y;

  glm::mat4 inverse_view;

  glm::vec2 mouse_world_pos;

  //float mouse_world_pos_x;
  //float mouse_world_pos_y;

  void getMouseInput();

  void getMouseWorldPos();

  void inputKeyboard(Player &player);

  void setApp(App &app)
  {
    app_ptr = &app;
  }

};

#endif
