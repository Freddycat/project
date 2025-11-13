
// input.cpp

#include "input.h"
#include "app.h"
#include "camera.h"
#include "player.h"
#include <SDL3/SDL.h>

Mouse & Input::GetMouse()
{
  return mouse;
}

void Input::GetMouseInput()
{
  SDL_GetMouseState(&mouse.screen_pos.x, &mouse.screen_pos.y);
  mouse.center_pos = mouse.screen_pos - g.window_center;
}

void Input::GetMouseWorldPos()
{
  float ndc_x = (2.0f * mouse.screen_pos.x) / g.window_width - 1.0f;
  float ndc_y = 1.0f - (2.0f * mouse.screen_pos.y) / g.window_height;

  glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);
  glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f);

  glm::vec4 ray_near_world = camera.inverse_view * ray_clip;
  glm::vec4 ray_far_world = camera.inverse_view * ray_far_clip;
  ray_near_world /= ray_near_world.w;
  ray_far_world /= ray_far_world.w;

  glm::vec3 origin = glm::vec3(ray_near_world);
  glm::vec3 dir = glm::normalize(glm::vec3(ray_far_world - ray_near_world));
  float t = -origin.z / dir.z;
  glm::vec3 world_pos = origin + t * dir;
  mouse.world_pos.x = world_pos.x;
  mouse.world_pos.y = world_pos.y;
}

void Input::InputKeyboard()
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