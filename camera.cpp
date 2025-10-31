#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"

void Camera::centerCam(App &app, Dot &dot, Input &input)
{

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float midPoint_x = dot.posX + m_camera_delta_x * cam_zoom_amount;
  float midPoint_y = dot.posY + m_camera_delta_y * cam_zoom_amount;

  cam_pos_x = midPoint_x - app.window_center_x;
  cam_pos_y = midPoint_y - app.window_center_y;

  glTranslatef(-cam_pos_x, -cam_pos_y, 0); // move the "view"
}