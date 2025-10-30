#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <app.h>

struct Input
{

  float mouse_pos_x;
  float mouse_pos_y;

  void inputMouse()
  {
    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
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
