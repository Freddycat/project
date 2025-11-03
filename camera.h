#ifndef CAMERA_H
#define CAMERA_H

struct App;
struct Player;
struct Input;

struct Camera
{
  App *app_ptr = nullptr;
  float center_x;
  float center_y;
  float origin_x;
  float origin_y;
  float zoom_amount = 0.3f;

  void centerCam(Input &input, Player &player);

  void setApp(App &app)
  {
    app_ptr = &app;
  }
};

#endif