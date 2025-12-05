#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <glm/glm.hpp>
#include "player.h"
#include "playerCtx.h"
#include "camera.h"
#include <collisions.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

void Player::SetOrientation(Camera &cam)
{
    cam_forward = cam.map_forward;
    cam_right = cam.right;
    cam_up = cam.up;
}

void Player::MovePlayer(float time_elapsed, PlayerCtx &ctx, ColliderCtx &colliderCtx)
{

    // Always rotate player's wish_dir to wishdir
    // float theta = glm::radians(orientation);

    vec3 indir;
    indir.x = input_direction.x;
    indir.y = input_direction.y;
    indir.z = 0.0;

    glm::vec3 wishdir = indir.x * cam_right + indir.y * cam_forward;
    
    if (glm::length2(wishdir) > 0.0f)
        wishdir = glm::normalize(wishdir);

    // Acceleration is applied to velocity
    velocity += glm::vec3(wishdir.x, wishdir.y, 0.0f) * accel;

    // Maximim movement amount for this frame
    glm::vec3 distance = velocity * time_elapsed;

    // Head pos
    //shoulder_height = position + glm::vec3(0.0f, 0.0f, height * 0.75);

    // Test collisions
    // **look into more voids less collisions**
    CollisionResult collision = TestCollisions(colliderCtx, position, position + distance, position, shoulder_height, radius);

    // Move either full distance or portion till collision
    glm::vec3 delta = distance * collision.fraction;

    if (collision.hit)
    { // Slide along collision normal
        glm::vec3 remaining = distance * (1.0f - collision.fraction);
        remaining -= glm::dot(remaining, collision.normal) * collision.normal;
        delta += remaining;
        position += collision.normal * 0.1f; // push you back off the wall slightly before more movement
    }

    // Apply movement
    position += delta;
    shoulder_height = position + glm::vec3(0.0f, 0.0f, cam_center);

    speed = glm::length(velocity);

    // Friction
    velocity.x *= 0.8f;
    velocity.y *= 0.8f;
    if (std::abs(velocity.x) < 0.1f)
        velocity.x = 0.0f;
    if (std::abs(velocity.y) < 0.1f)
        velocity.y = 0.0f;
    // update player ctxt for other functions
    ctx.pos = shoulder_height;
}

void Player::UpdatePlayerCap(PlayerCtx &ctx, Gizmos &gizmos, std::vector<Point> &points, std::vector<Shape> &capsules)
{
    static vec4 color = {1, 0, 0, 1};
    capsules[0].center = {position.x, position.y, 16.0f};
    facing_line_start.pos = ctx.pos;
    vec3 endpos = ctx.pos + ctx.facing * 100.0f;
    facing_line_end.pos = endpos;
    facing_line_start.color = color;
    facing_line_end.color = color;
    gizmos.line_points.push_back(facing_line_start);
    gizmos.line_points.push_back(facing_line_end);
}

void Player::UpdateCrosshair(std::vector<Point> &points, glm::vec3 pos, PlayerCtx &ctx)
{
    Point xhair;

    xhair.pos = pos;
    xhair.color = glm::vec4(1, 1, 1, 1);

    points.push_back(xhair);

    ctx.xhair = pos;
    auto distance = ctx.xhair - ctx.pos;
    ctx.facing = glm::normalize(distance);
}
