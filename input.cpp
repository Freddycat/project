
// input.cpp

#include "input.h"
#include "app.h"
#include "camera.h"
#include "player.h"
#include "playerCtx.h"
#include "collisions.h"
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

    float win_w = g.window_width;
    float win_h = g.window_height;

    float ndc_x = (2.0f * mouse.screen_pos.x) / win_w - 1.0f;
    float ndc_y = 1.0f - (2.0f * mouse.screen_pos.y) / win_h;

    glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);    // these just clip screen space
    glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f); // these just clip screen space
    glm::vec4 ray_near_world = camera.inverse_view * ray_clip;
    glm::vec4 ray_far_world = camera.inverse_view * ray_far_clip;

    ray_near_world /= ray_near_world.w;
    ray_far_world /= ray_far_world.w;

    vec3 origin = glm::vec3(ray_near_world);

    glm::dvec3 near_point = glm::unProject(
        glm::dvec3(mouse.screen_pos.x, win_h - mouse.screen_pos.y, 0.0f),
        camera.view,
        camera.projection,
        glm::dvec4(0, 0, g.window_width, win_h));

    glm::dvec3 far_point = glm::unProject(
        glm::dvec3(mouse.screen_pos.x, win_h - mouse.screen_pos.y, 1.0f),
        camera.view,
        camera.projection,
        glm::dvec4(0, 0, g.window_width, win_h));

    vec3 dist = far_point - near_point;

    glm::dvec3 direction = glm::normalize(dist);

    // vec3 direction = glm::normalize(glm::vec3(ray_far_world - ray_near_world));

    glm::dvec3 ground_start{10000.0, 10000.0, 0.0};
    glm::dvec3 ground_end{-10000.0, -10000.0, -2.0};

    float range = 4000.0;

    CollisionResult ground_hit = RayHit(near_point, direction, ground_start, ground_end, range);

    if (ground_hit.hit)
    std::cout << " hit! " << std::endl;

    double distance = range * ground_hit.fraction;

    glm::dvec3 world_pos = near_point + distance * direction;
    glm::vec3 head_pos = glm::vec3{world_pos.x, world_pos.y, world_pos.z + offset};
    glm::vec3 up{0, 0, 1};
    double t = (offset - near_point.z) / direction.z;
    glm::vec3 xhair_pos = near_point + t * direction;

    // float distance = -origin.z / direction.z;

    float distance_offset = (offset - origin.z) / direction.z;
    float camera_offset = (camera.position.z - origin.z) / direction.z;
    //glm::vec3 offset_pos = origin + distance_offset * direction;

    mouse.world_pos = world_pos;
    mouse.xhair_pos = xhair_pos;
    mouse.camera_pos = near_point;
    mouse.cam_to_mouse = direction;
}

void Input::InputKeyboard(Player &player)
{

    const bool *keyboard = SDL_GetKeyboardState(nullptr);

    player.input_direction = {0.0f, 0.0f};

    if (keyboard[SDL_SCANCODE_W])
        player.input_direction.y -= 1.0f;

    if (keyboard[SDL_SCANCODE_S])
        player.input_direction.y += 1.0f;

    if (keyboard[SDL_SCANCODE_A])
        player.input_direction.x -= 1.0f;

    if (keyboard[SDL_SCANCODE_D])
        player.input_direction.x += 1.0f;

    if (glm::length2(player.input_direction) > 0.0f)
        player.input_direction = glm::normalize(player.input_direction);
}