#include "input.h"
#include <SDL3/SDL.h>

void input(std::vector<Dot> &dots)
{
  const bool *keyboard = SDL_GetKeyboardState(nullptr);

  if (keyboard[SDL_SCANCODE_UP])
  {
    for (auto &dot : dots)
    {
      dot.posY -= dot.speed;
      // dot->posY -= dot->speed;
    }
  }

  if (keyboard[SDL_SCANCODE_DOWN])
  {
    for (auto &dot : dots)
    {
      dot.posY += dot.speed;
      // dot->posY += dot->speed;
    }
  }

  if (keyboard[SDL_SCANCODE_LEFT])
  {
    for (auto &dot : dots)
    {
      dot.posX -= dot.speed;
      // dot->posX -= dot->speed;
    }
  }

  if (keyboard[SDL_SCANCODE_RIGHT])
  {
    for (auto &dot : dots)
    {
      dot.posX += dot.speed;
      // dot->posX += dot->speed;
    }
  }
}