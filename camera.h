#ifndef CAMERA_H
#define CAMERA_H

struct App;
struct Dot;
struct Input;

struct Camera
{
  float m_camera_delta_x;
  float m_camera_delta_y;
  float cam_pos_x;
  float cam_pos_y;
  float cam_zoom_amount = 0.3f;
  void centerCam(App &app, Dot &dot, Input &input);
};

#endif