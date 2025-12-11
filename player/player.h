#ifndef PLAYER_H
#define PLAYER_H

#include "models.h"
#include <entt/entt.hpp>
#include <gizmos.h>
#include <vector>

struct World;
struct Input;
struct Blast;
struct Beam;
struct Weapon;
struct Camera;
struct PlayerCtx;
struct ColliderCtx;

using glm::vec2, glm::vec3;

struct Player {
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;

    vec2 input_direction;

    // - positions
    vec3 position = {0.0, 0.0, 0.0};
    vec3 velocity = {0.0, 0.0, 0.0};
    vec3 direction = {0.0, 0.0, 0.0};
    vec3 next_pos = {0.0, 0.0, 0.0};
    vec3 cam_forward = {0.0, 0.0, 0.0};
    vec3 cam_right = {0.0, 0.0, 0.0};
    vec3 cam_up = {0.0, 0.0, 0.0};

    vec3 shoulder_height = {0.0, 0.0, 24.0};

    float speed = 0.0f;

    float accel = 120.0f;

    float radius = 16.0f;
    float height = 32.0f;

    float center = height / 2;

    float cam_center = height * 0.75f;

    vector<Weapon> weapons;
    vector<Model> models;
    vector<Transform> transforms;

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