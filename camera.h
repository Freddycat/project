#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct App;
struct Player;
struct Input;

struct Camera
{
  App *app_ptr = nullptr;

  glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 camera_offset = glm::vec3(500.0f, 500.0f, 500.0f);
  glm::vec3 up = glm::vec3(0, 0, -1);

  glm::mat4 view_matrix{1.0f};

  float center_x;
  float center_y;
  float origin_x;
  float origin_y;
  float zoom_amount = 0.3f;

  void centerCam(Input &input, Player &player);
  void isoCam();

  void setApp(App &app)
  {
    app_ptr = &app;
  }
};

#endif