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

  glGraphics::Shader::setMat4(shaderID, "projection", glm::value_ptr(projection));
  glGraphics::Shader::setMat4(shaderID, "view", glm::value_ptr(view));
}

void Camera::CenterCam(GLuint &shaderID, Input &input, Player &player)
{
  glm::vec3 mouse_zoomed(input.mouse_center_pos * zoom_amount, 0.0f);

  target = glm::vec3(player.pos_x, player.pos_y, 0.0f);
  position = target + offset; // set position initially

  forward = glm::normalize(target - position);
  right = glm::normalize(glm::cross(forward, up_axis));
  up = glm::normalize(glm::cross(right, forward));
  map_up = glm::vec3(-right.y, right.x, 0.0f);
  map_forward = glm::normalize(glm::vec3(forward.x, forward.y, 0.0f));

  glm::vec3 rotated_mouse = // mouse pos rotated
      right * mouse_zoomed.x + up * mouse_zoomed.y;

  glm::vec3 rotated_center = // window center rotated
      right * window_center.x + up * window_center.y;

  target = target + rotated_mouse - rotated_center; // modify target then add offset
  position = target + offset;                       // Camera position with offset

  view = glm::lookAt(position, // Camera position
                     target,   // Look at target
                     up_axis); // World up vector
  inverse_view = glm::inverse(projection * view);

  glGraphics::Shader::setMat4(shaderID, "view", glm::value_ptr(view));
}