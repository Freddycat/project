#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::centerCam(Input &input, Player &player)
{

  glm::vec3 mouse_offset_world(input.mouse_center_pos.x * zoom_amount,
                               input.mouse_center_pos.y * zoom_amount,
                               0.0f);

  glm::vec3 target = glm::vec3(player.pos_x, player.pos_y, 0.0f);
  target = target;
  position = target + offset;

  glm::vec3 forward = glm::normalize(target - position);
  glm::vec3 right = glm::normalize(glm::cross(forward, up_axis));
  glm::vec3 up = glm::normalize(glm::cross(right, forward));
  glm::vec3 adjusted_offset = right * mouse_offset_world.x + up * mouse_offset_world.y;

  glm::vec3 rotated_center =
      right * window_center.x +
      up * + window_center.y;

  target = target + adjusted_offset - rotated_center;
  position = target + offset;  // Camera position with offset
  view = glm::lookAt(position, // Camera position
                     target,   // Look at target
                     up);      // World up vector

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(view));
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