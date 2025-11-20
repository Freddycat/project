#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <gizmos.h>
#include <entt/entt.hpp>

struct World;
struct Input;
struct Blast;
struct Laser;
struct Weapon;

struct Player
{
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;

    glm::vec2 wish_dir;

    glm::vec3 pos;

    glm::vec3 velocity;

    glm::vec3 move_direction;

    glm::vec3 facing;

    glm::vec3 next_pos;

    glm::vec3 head_pos;

    float speed = 0.0f;

    float accel = 120.0f;

    float orientation = -45.0f;

    float radius = 16.0f;
    float height = 32.0f;

    float center = height / 2;

    float cam_center = height * 0.75f;

    entt::entity weapon;

    Point pos_dot;
    Point xhair_dot;

    Capsule playerCap{
        {0.0, 0.0, 0.0, 0.0},
        {radius, radius, height, 0.0},
        {0.8, 0.8, 0.8, 1.0}};

    void MovePlayer(float time_elapsed);
    void UpdatePlayerDot(std::vector<Point> &points, std::vector<Capsule> &capsules);
    void UpdateCrosshair(std::vector<Point> &points, glm::vec3 &pos);
};

#endif