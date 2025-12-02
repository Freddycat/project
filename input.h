#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <glm/glm.hpp>

struct Camera;
struct Player;
struct PlayerCtx;

using glm::vec2;
using glm::vec3;

struct Mouse
{
    vec2 screen_pos;
    vec2 center_pos;
    vec3 world_pos = {0.0f, 0.0f, 0.0f};
    vec3 xhair_pos = {0.0f, 0.0f, 0.0f};
    vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    vec3 cam_to_mouse = {0.0f, 0.0f, 0.0f};
    float ray_range = 2000;
};

struct Input
{
    static Input *Instance()
    {
        static Input input;
        return &input;
    }

    Mouse &GetMouse();

    void GetMouseInput(PlayerCtx &ctx);

    void GetMouseWorldPos(Camera &camera, float offset = 0.0f);

    void InputKeyboard(Player &player);
    void InputMouse(Player &player);

    Mouse mouse;
};

#endif
