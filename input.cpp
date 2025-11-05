
// input.cpp

#include "input.h"
#include "app.h"
#include "camera.h"
#include <SDL3/SDL.h>

void Input::getMouseInput()
{
  SDL_GetMouseState(&mouse_screen_pos_x, &mouse_screen_pos_y);
}

void Input::getMouseWorldPos()
{
  mouse_world_pos_x = app_ptr->cam.origin_x + mouse_screen_pos_x;
  mouse_world_pos_y = app_ptr->cam.origin_y + mouse_screen_pos_y;
}

void Input::inputKeyboard(Player &player)
{

  const bool *keyboard = SDL_GetKeyboardState(nullptr);

  if (keyboard[SDL_SCANCODE_W])

    player.velocity_y -= player.speed;

  if (keyboard[SDL_SCANCODE_S])

    player.velocity_y += player.speed;

  if (keyboard[SDL_SCANCODE_A])

    player.velocity_x -= player.speed;

  if (keyboard[SDL_SCANCODE_D])

    player.velocity_x += player.speed;
}