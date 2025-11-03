#include "camera.h"
#include "glad/glad.h"
#include "app.h"
#include "input.h"

void Camera::centerCam(Input &input, Player &player)
{

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float window_offset_x = input.mouse_screen_pos_x - app_ptr->window_center_x;
  float window_offset_y = input.mouse_screen_pos_y - app_ptr->window_center_y;

  center_x = player.pos_x + window_offset_x * zoom_amount;
  center_y = player.pos_y + window_offset_y * zoom_amount;

  origin_x = center_x - app_ptr->window_center_x;
  origin_y = center_y - app_ptr->window_center_y;

  glTranslatef(-origin_x, -origin_y, 0); // move the "view"
}