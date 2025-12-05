#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <gizmos.h>
#include <entt/entt.hpp>

struct World;
struct Input;
struct Blast;
struct Beam;
struct Weapon;
struct Camera;
struct PlayerCtx;
struct ColliderCtx;

using glm::vec2, glm::vec3;

struct Player
{
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;

    vec2 input_direction;

    vec3 // - positions
        position,
        velocity,
        direction,
        next_pos,
        shoulder_height,
        cam_forward,
        cam_right,
        cam_up;

    float speed = 0.0f;

    float accel = 120.0f;

    float radius = 16.0f;
    float height = 32.0f;

    float center = height / 2;

    float cam_center = height * 0.75f;

    bool mouse1;

    entt::entity weapon;
    vector<Weapon> weapons;

    Point pos_dot;
    Point xhair_dot;
    Point facing_line_start;
    Point facing_line_end;

    Shape playerCap{
        {0.0, 0.0, 0.0},
        {radius, radius, height},
        {0.8, 0.8, 0.8, 1.0}};

    void SetOrientation(Camera &cam);
    void MovePlayer(float time_elapsed, PlayerCtx &ctx, ColliderCtx &colliderCtx);
    void UpdatePlayerCap(PlayerCtx &ctx, Gizmos &gizmos, std::vector<Point> &points, std::vector<Shape> &capsules);
    void UpdateCrosshair(std::vector<Point> &points, glm::vec3 pos, PlayerCtx &ctx);
};

#endif