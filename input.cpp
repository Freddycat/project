
// input.cpp

#include "input.h"
#include "app.h"
#include "camera.h"
#include "player.h"
#include "playerCtx.h"
#include <SDL3/SDL.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

Mouse &Input::GetMouse()
{
    return mouse;
}

void Input::GetMouseInput(PlayerCtx &player)
{
    Uint32 buttons = SDL_GetMouseState(&mouse.screen_pos.x, &mouse.screen_pos.y);
    mouse.center_pos = mouse.screen_pos - g.window_center;

    bool left = (buttons & SDL_BUTTON_LMASK) != 0;
    bool right = (buttons & SDL_BUTTON_RMASK) != 0;
    bool middle = (buttons & SDL_BUTTON_MMASK) != 0;

    if (left)
    {
        player.firing = true;
    }
    else
    {
        player.firing = false;
    }
}

void Input::GetMouseWorldPos(Camera &camera, float offset)
{
    float ndc_x = (2.0f * mouse.screen_pos.x) / g.window_width - 1.0f;
    float ndc_y = 1.0f - (2.0f * mouse.screen_pos.y) / g.window_height;

    glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);    // these just clip screen space
    glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f); // these just clip screen space
    glm::vec4 ray_near_world = camera.inverse_view * ray_clip;
    glm::vec4 ray_far_world = camera.inverse_view * ray_far_clip;
    ray_near_world /= ray_near_world.w;
    ray_far_world /= ray_far_world.w;

    glm::vec3 origin = glm::vec3(ray_near_world);
    glm::vec3 direction = glm::normalize(glm::vec3(ray_far_world - ray_near_world));
    float distance_world = -origin.z / direction.z;
    float distance_offset = (offset - origin.z) / direction.z;
    float camera_offset = (camera.position.z - origin.z) / direction.z;
    glm::vec3 world_pos = origin + distance_world * direction;
    glm::vec3 offset_pos = origin + distance_offset * direction;
    
    mouse.world_pos = world_pos;
    mouse.xhair_pos = offset_pos;
    mouse.camera_pos = origin;
    mouse.cam_to_mouse = direction;
}

void Input::InputKeyboard(Player &player)
{

    const bool *keyboard = SDL_GetKeyboardState(nullptr);

    player.wish_dir = {0.0f, 0.0f};

    if (keyboard[SDL_SCANCODE_W])
        player.wish_dir.y -= 1.0f;

    if (keyboard[SDL_SCANCODE_S])
        player.wish_dir.y += 1.0f;

    if (keyboard[SDL_SCANCODE_A])
        player.wish_dir.x -= 1.0f;

    if (keyboard[SDL_SCANCODE_D])
        player.wish_dir.x += 1.0f;

    if (glm::length2(player.wish_dir) > 0.0f)
        player.wish_dir = glm::normalize(player.wish_dir);
}