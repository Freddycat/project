#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <glm/glm.hpp>

struct App;
struct Player;
struct Camera;
struct State;

struct Input
{
  glm::vec2 window_center;

  glm::vec2 mouse_screen_pos;

  glm::vec2 mouse_center_pos;

  // float mouse_screen_pos_x;
  // float mouse_screen_pos_y;

  glm::mat4 inverse_view;

  glm::vec2 mouse_world_pos;

  // float mouse_world_pos_x;
  // float mouse_world_pos_y;

  void GetMouseInput();

  void GetMouseWorldPos(State &state, Camera &cam);

  void InputKeyboard(Player &player);
};

#endif
