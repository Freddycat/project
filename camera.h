#ifndef CAMERA_H
#define CAMERA_H

struct App;
struct Player;
struct Input;

struct Camera
{
  App *app_ptr = nullptr;
  float m_camera_delta_x;
  float m_camera_delta_y;
  float cam_pos_x;
  float cam_pos_y;
  float cam_zoom_amount = 0.3f;

  void centerCam(Player &player);

  void setApp(App &app)
  {
    app_ptr = &app;
  }
};

#endif