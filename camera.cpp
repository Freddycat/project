#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"
#include "playerCtx.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::SetCam()
{
    /* ortho::
     projection = glm::ortho(
         0.0f, g.set_width,  // left, right
         g.set_height, 0.0f, // bottom, top
         -1.0f, 2000.0f      // near, far
     ); */

    glm::mat4 perspective = glm::perspective(
        glm::radians(12.0f), g.set_width / g.set_height, 0.1f, 5000.0f);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0));

    projection = perspective * scale;

    target = vec3{0.0, 0.0, 0.0};
    position = target + offset; // set position initially

    forward = glm::normalize(position - target);
    right = glm::normalize(glm::cross(up_axis, forward));
    up = glm::cross(right, forward);
    map_forward = glm::normalize(glm::vec3(forward.x, forward.y, 0.0f));

    glm::vec4 center_clip(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 center_near_world = inverse_view * center_clip;
    center_near_world /= center_near_world.w;
    glm::vec3 center_origin = glm::vec3(center_near_world);

    glm::vec4 center_far_clip(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 center_far_world = inverse_view * center_far_clip;
    center_far_world /= center_far_world.w;
    glm::vec3 center_dir = glm::normalize(glm::vec3(center_far_world - center_near_world));
    float center_dist = -center_origin.z / center_dir.z;
    center_world_pos = center_origin + center_dist * center_dir;
}

void Camera::CenterCam(Input &input, PlayerCtx &player)
{

    glm::vec2 mousepos = input.mouse.center_pos;

    glm::vec3 mouse_zoomed(mousepos * zoom_amount, 0.0f);

    // target = player.pos;
    target = player.pos;
    position = target + offset; // set position initially

    /* for ortho::
    forward = glm::normalize(target - position);
    right = glm::normalize(glm::cross(forward, up_axis));
    up = glm::normalize(glm::cross(right, forward));
    */

    // forward = glm::normalize(position - target);
    // right = glm::normalize(glm::cross(up_axis, forward));
    // up = glm::cross(right, forward);
    // map_forward = glm::normalize(glm::vec3(forward.x, forward.y, 0.0f));
    
    glm::vec3 rotated_mouse = // mouse pos rotated
        right * mouse_zoomed.x + map_forward * mouse_zoomed.y;

    glm::vec3 rotated_center = // window center rotated
        right * g.window_center.x + map_forward * g.window_center.y;

    target = target + rotated_mouse /* - rotated_center */; // modify target then add offset
    position = target + offset;                             // Camera position with offset

    view = glm::lookAt(position, // Camera position
                       target,   // Look at target
                       up_axis); // World up vector
    inverse_view = glm::inverse(projection * view);
}