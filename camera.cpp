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

  glm::vec3 mouse_offset_world(window_offset_x * zoom_amount,
                               window_offset_y * zoom_amount,
                               0.0f);
                               
  glm::vec3 target = glm::vec3(player.pos_x, player.pos_y, 0.0f);

  glm::vec3 camera_pos = target + camera_offset;
  glm::vec3 forward = glm::normalize(target - camera_pos);
  glm::vec3 right = glm::normalize(glm::cross(forward, up));
  glm::vec3 adjusted_offset = right * mouse_offset_world.x + up * mouse_offset_world.y;

  //camera_target = target /* + adjusted_offset */;
  //camera_pos = camera_target + camera_offset; // Camera position with offset
  view_matrix = glm::lookAt(camera_pos,       // Camera position
                            camera_target,    // Look at target
                            up);              // World up vector

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(view_matrix));
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