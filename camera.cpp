#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::centerCam(Input &input, Player &player)
{
    float window_offset_x = input.mouse_screen_pos_x - app_ptr->window_center_x;
    float window_offset_y = input.mouse_screen_pos_y - app_ptr->window_center_y;

    glm::vec3 mouse_world_pos(window_offset_x * zoom_amount, window_offset_y * zoom_amount, 0.0f);
    glm::vec2 mouse_offset(window_offset_x * zoom_amount, window_offset_y * zoom_amount);

    glm::vec2 offset = glm::vec2(player.pos_x, player.pos_y) - mouse_offset;

    // Step 3: rotate offset to match isometric axes
    float angle = glm::radians(45.0f);
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    glm::vec2 rotated_mouse;
    rotated_mouse.x = mouse_offset.x * cosA - mouse_offset.y * sinA;
    rotated_mouse.y = mouse_offset.x * sinA + mouse_offset.y * cosA;

    glm::vec2 rotated_offset;
    rotated_offset.x = offset.x * cosA - offset.y * sinA;
    rotated_offset.y = offset.x * sinA + offset.y * cosA;

    center_x = rotated_offset.x + rotated_mouse.x;
    center_y = rotated_offset.y + rotated_mouse.y;

    //center_x = player.pos_x + mouse_world_pos.x;
    //center_y = player.pos_y + mouse_world_pos.y;

    origin_x = center_x - app_ptr->window_center_x;
    origin_y = center_y - app_ptr->window_center_y;

    std::cout << "Camera Center: (" << center_x << ", " << center_y << ")\n";

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(camera_pos.x - origin_x, camera_pos.y - origin_y, camera_pos.z);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 0, 1);
}

/*
void Camera::isoCam()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -500);
  glRotatef(-45, 1, 0, 0); // tilt up from top-down
  glRotatef(45, 0, 1, 0);  // view from top-right corner
} */