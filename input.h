#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <glm/glm.hpp>

struct Camera;
struct Player;
struct PlayerCtx;

struct Mouse
{
  glm::vec2 screen_pos;
  glm::vec2 center_pos;
  glm::vec3 world_pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 xhair_pos = {0.0f, 0.0f, 0.0f};
};

struct Input
{
  static Input *Instance()
  {
    static Input input;
    return &input;
  }

  Mouse &GetMouse();

  void GetMouseInput(PlayerCtx &ctx);

  void GetMouseWorldPos(Camera &camera, float offset = 0.0f);

  void InputKeyboard(Player &player);
  void InputMouse(Player &player);

  Mouse mouse;
};


#endif
