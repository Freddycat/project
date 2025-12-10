#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <vector>

struct Camera;
struct Player;
struct PlayerCtx;

using glm::vec2;
using glm::vec3;

struct Mouse
{
    vec2 screen_pos;
    vec2 center_pos;
    glm::vec3 world_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 xhair_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 cam_to_mouse = {0.0f, 0.0f, 0.0f};
    float ray_range = 5000;
    bool click_l = false;
    bool click_r = false;
};

struct Keyboard{
    bool mod_shift = false;
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

    Keyboard KB;
    Mouse mouse;
};

#endif
