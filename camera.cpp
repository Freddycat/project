#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::SetCam(GLuint &shaderID)
{
  projection = glm::ortho(
      0.0f, 800.0f,     // left, right
      600.0f, 0.0f,     // bottom, top
      -1000.0f, 2000.0f // near, far
  );

  Gfx::setMat4(shaderID, "projection", glm::value_ptr(projection));
  Gfx::setMat4(shaderID, "view", glm::value_ptr(view));
}

void Camera::CenterCam(GLuint &shaderID, Input &input, Player &player)
{

  glm::vec3 mouse_zoomed(input.mouse.center_pos * zoom_amount, 0.0f);

  target = glm::vec3(player.pos.x, player.pos.y, player.cam_center);
  position = target + offset; // set position initially

  forward = glm::normalize(target - position);
  right = glm::normalize(glm::cross(forward, up_axis));
  up = glm::normalize(glm::cross(right, forward));
  map_up = glm::vec3(-right.y, right.x, 0.0f);
  map_forward = glm::normalize(glm::vec3(forward.x, forward.y, 0.0f));

  glm::vec3 rotated_mouse = // mouse pos rotated
      right * mouse_zoomed.x + up * mouse_zoomed.y;

  glm::vec3 rotated_center = // window center rotated
      right * g.window_center.x + up * g.window_center.y;

  target = target + rotated_mouse - rotated_center; // modify target then add offset
  position = target + offset;                       // Camera position with offset

  view = glm::lookAt(position, // Camera position
                     target,   // Look at target
                     up_axis); // World up vector
  inverse_view = glm::inverse(projection * view);

  Gfx::setMat4(shaderID, "view", glm::value_ptr(view));
}