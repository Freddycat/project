#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::SetCam()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  projection = glm::ortho(
      0.0f, 800.0f,     // left, right
      600.0f, 0.0f,     // bottom, top
      -1000.0f, 2000.0f // near, far
  );

  glOrtho(0, 800, 600, 0, -1000, 2000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_POINT_SMOOTH);
}

void Camera::CenterCam(Input &input, Player &player)
{

  glm::vec3 mouse_zoomed(input.mouse_center_pos * zoom_amount, 0.0f);

  target = glm::vec3(player.pos_x, player.pos_y, 0.0f);
  position = target + offset;

  forward = glm::normalize(target - position);
  right = glm::normalize(glm::cross(forward, up_axis));
  up = glm::normalize(glm::cross(right, forward));
  map_up = glm::vec3(-right.y, right.x, 0.0f);
  map_forward = glm::normalize(glm::vec3(forward.x, forward.y, 0.0f));
  glm::vec3 rotated_mouse = right * mouse_zoomed.x + up * mouse_zoomed.y;
  // adjusted_offset.z = 0.0f;

  glm::vec3 rotated_center =
      right * window_center.x +
      up * window_center.y;

  target = target + rotated_mouse - rotated_center;
  position = target + offset;  // Camera position with offset
  
  view = glm::lookAt(position, // Camera position
                     target,   // Look at target
                     up_axis); // World up vector

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(view));
}