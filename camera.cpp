#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"

void Camera::centerCam(Player &player)
{

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float midPoint_x = player.posX + m_camera_delta_x * cam_zoom_amount;
  float midPoint_y = player.posY + m_camera_delta_y * cam_zoom_amount;

  cam_pos_x = midPoint_x - app_ptr->window_center_x;
  cam_pos_y = midPoint_y - app_ptr->window_center_y;

  glTranslatef(-cam_pos_x, -cam_pos_y, 0); // move the "view"
}