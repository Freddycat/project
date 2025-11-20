#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <glm/glm.hpp>
#include "player.h"
#include "app.h"
#include "world.h"
#include <collisions.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void Player::MovePlayer(float time_elapsed)
{

    // Always rotate player's wish_dir to wishdir
    float theta = glm::radians(orientation);
    glm::vec2 wishdir;
    wishdir.x = wish_dir.x * cos(theta) - wish_dir.y * sin(theta);
    wishdir.y = wish_dir.x * sin(theta) + wish_dir.y * cos(theta);

    if (glm::length2(wishdir) > 0.0f)
        wishdir = glm::normalize(wishdir);

    // Acceleration is applied to velocity
    velocity += glm::vec3(wishdir.x, wishdir.y, 0.0f) * accel;

    // Maximim movement amount for this frame
    glm::vec3 distance = velocity * time_elapsed;

    // Head pos
    head_pos = pos + glm::vec3(0.0f, 0.0f, height);

    // Test collisions
    // **look into more voids less collisions**
    CollisionResult collision = TestCollisions(pos, pos + distance, pos, head_pos, radius);

    // Move either full distance or portion till collision
    glm::vec3 delta = distance * collision.fraction;

    if (collision.hit)
    { // Slide along collision normal
        glm::vec3 remaining = distance * (1.0f - collision.fraction);
        remaining -= glm::dot(remaining, collision.normal) * collision.normal;
        delta += remaining;
        pos += collision.normal * 0.1f; //push you back off the wall slightly before more movement
    }

    // Apply movement
    pos += delta;

    speed = glm::length(velocity);

    // Friction
    velocity.x *= 0.8f;
    velocity.y *= 0.8f;
    if (std::abs(velocity.x) < 0.1f)
        velocity.x = 0.0f;
    if (std::abs(velocity.y) < 0.1f)
        velocity.y = 0.0f;
}

void Player::UpdatePlayerDot(std::vector<Point> &points, std::vector<Capsule> &capsules)
{
    //points[0].pos = {pos.x, pos.y, 0.0f};
    //points[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    capsules[0].center = {pos.x, pos.y, 16.0f, 0.0f};
}

void Player::UpdateCrosshair(std::vector<Point> &points, glm::vec3 &pos)
{
    Point xhair;

    xhair.pos = pos;
    xhair.color = glm::vec3(1.0f, 1.0f, 1.0f);

    points.push_back(xhair);
}
