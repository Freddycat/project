#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <app.h>

struct Input
{

  float m_screen_pos_x;
  float m_screen_pos_y;
  float m_world_pos_x;
  float m_world_pos_y;

  void inputMouse(App &app)
  {
    SDL_GetMouseState(&m_screen_pos_x, &m_screen_pos_y);

    m_world_pos_x = m_screen_pos_x - app.window_center_x;
    m_world_pos_y = m_screen_pos_y - app.window_center_y;
  }

  void inputKeyboard(Dot &dot)
  {

    const bool *keyboard = SDL_GetKeyboardState(nullptr);

    if (keyboard[SDL_SCANCODE_W])

      dot.posY -= dot.speed;

    if (keyboard[SDL_SCANCODE_S])

      dot.posY += dot.speed;

    if (keyboard[SDL_SCANCODE_A])

      dot.posX -= dot.speed;

    if (keyboard[SDL_SCANCODE_D])

      dot.posX += dot.speed;
  }
};

#endif
