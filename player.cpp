#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <glm/glm.hpp>
#include "player.h"
#include "app.h"
#include "world.h"

#include <iostream>

void Player::MovePlayer()
{

    float theta = glm::radians(orientation); // -45 degrees

    // Rotate velocity vector by orientation
    float rotated_vx = vel.x * cos(theta) - vel.y * sin(theta);
    float rotated_vy = vel.x * sin(theta) + vel.y * cos(theta);

    pos.x += rotated_vx;
    pos.y += rotated_vy;

    // Apply friction to gradually stop the player
    vel.x *= 0.8f; // Friction factor for x
    vel.y *= 0.8f; // Friction factor for y

    // If velocity is very small, set it to zero to prevent drifting
    if (std::abs(vel.x) < 0.1f)
        vel.x = 0.0f;
    if (std::abs(vel.y) < 0.1f)
        vel.y = 0.0f;
}

void Player::UpdatePlayerDot(std::vector<Point> &points, std::vector<Capsule> &capsules)
{
    points[0].pos = {pos.x, pos.y, 0.0f};
    points[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    capsules[0].center = {pos.x, pos.y, 16.0f, 0.0f};
}

void Player::UpdateCrosshair(std::vector<Point> &points, glm::vec2 pos)
{
    points[1].pos = {pos.x, pos.y, 0.0f};
    points[1].color = glm::vec3(1.0f, 1.0f, 1.0f);
}
