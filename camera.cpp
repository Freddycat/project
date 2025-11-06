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

  glm::vec3 mouse_offset_world(input.mouse_center_pos.x * zoom_amount,
                               input.mouse_center_pos.y * zoom_amount,
                               0.0f);

  glm::vec3 target = glm::vec3(player.pos_x, player.pos_y, 0.0f);
  target = target;
  position = target + offset;

  glm::vec3 forward = glm::normalize(target - position);
  glm::vec3 right = glm::normalize(glm::cross(forward, up_axis));
  glm::vec3 up = glm::normalize(glm::cross(right, forward));
  glm::vec3 up_on_map = glm::vec3(-right.y, right.x, 0.0f);
  glm::vec3 adjusted_offset = right * mouse_offset_world.x + up_on_map * mouse_offset_world.y;
  // adjusted_offset.z = 0.0f;

  glm::vec3 rotated_center =
      right * window_center.x +
      up_axis * +window_center.y;

  target = target + adjusted_offset - rotated_center;
  position = target + offset;  // Camera position with offset
  view = glm::lookAt(position, // Camera position
                     target,   // Look at target
                     up_axis); // World up vector

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(view));
}