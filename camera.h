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

  float handle_x =  0.0f;

  float handle_y =  0.0f;

  glm::vec2 window_center = glm::vec2(0.0f, 0.0f);
  
  glm::vec3 position = glm::vec3(500.0f, 500.0f, 500.0f);
  glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 offset = glm::vec3(500.0f, 500.0f, 500.0f);
  glm::vec3 up_axis = glm::vec3(0, 0, -1);

  glm::mat4 projection{1.0f};
  glm::mat4 view{1.0f};

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