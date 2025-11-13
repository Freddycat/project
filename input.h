#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <glm/glm.hpp>

// struct Mouse;

struct Mouse
{
  glm::vec2 screen_pos;

  glm::vec2 center_pos;

  glm::vec3 world_pos = {0.0f, 0.0f, 0.0f};
};

struct Input
{
  static Input *Instance()
  {
    static Input input;
    return &input;
  }

  Mouse &GetMouse();

  void GetMouseInput();

  void GetMouseWorldPos();

  void InputKeyboard();

  Mouse mouse;
};


#endif
