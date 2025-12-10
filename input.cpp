
#include "input.h"
#include "camera.h"
#include "collisions.h"
#include "global.h"
#include "player.h"
#include "playerCtx.h"
#include <SDL3/SDL.h>
#include <print>

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

    if (left) {
        player.firing = true;
        mouse.click_l = true;
    } else {
        player.firing = false;
        mouse.click_l = false;
    }
    if (right) {
        mouse.click_r = true;
    } else {
        mouse.click_r = false;
    }
}

void Input::GetMouseWorldPos(Camera &camera, float offset)
{

    float win_w = g.window_width;
    float win_h = g.window_height;

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

    float range = 5000.0;
    // the range has to be tied to camera distance arbitrarily at some point

    // CollisionResult ground_hit = RayHitBox(near_point, direction, ground_start, ground_end, range);
    CollisionResult ground_hit;
    if (KB.mod_shift) {
        vec3 point{0.0};
        vec3 normal = camera.map_forward;
        CollisionResult plane_hit = RayHitPlane(near_point, direction, point, normal, range);
        ground_hit = plane_hit;
    } else {
        ground_hit = RayHitPlaneZ(near_point, direction, 0.0, range);
    }
    // if (!ground_hit.hit)
    //     std::print("MOUSE WORLD POS: no ground found! :(\n");

    double distance = range * ground_hit.fraction;
    //std::print("distance: {}\n", distance);

    glm::dvec3 world_pos = near_point + distance * direction;
    glm::vec3 head_pos = glm::vec3{world_pos.x, world_pos.y, world_pos.z + offset};
    glm::vec3 up{0, 0, 1};
    double t = (offset - near_point.z) / direction.z;
    glm::vec3 xhair_pos = near_point + t * direction;

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

    if (keyboard[SDL_SCANCODE_LSHIFT])
        KB.mod_shift = true;
    else
        KB.mod_shift = false;
}