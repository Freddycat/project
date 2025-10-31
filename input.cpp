#include "input.h"
#include "app.h"
#include "camera.h"
#include <SDL3/SDL.h>

void Input::getMouseInput()
{
  SDL_GetMouseState(&m_screen_pos_x, &m_screen_pos_y);

  app_ptr->cam.m_camera_delta_x = m_screen_pos_x - app_ptr->window_center_x;
  app_ptr->cam.m_camera_delta_y = m_screen_pos_y - app_ptr->window_center_y;
}

void Input::getMouseWorldPos()
{
  m_world_pos_x = app_ptr->cam.cam_pos_x + m_screen_pos_x;
  m_world_pos_y = app_ptr->cam.cam_pos_y + m_screen_pos_y;
}

void Input::inputKeyboard(Dot &dot)
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