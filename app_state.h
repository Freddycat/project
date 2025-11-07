#ifndef APP_STATE_H
#define APP_STATE_H

#include <glm/glm.hpp>

struct State
{
  bool running;

  glm::vec2 window_size;
  int window_width;
  int window_height;
  
  glm::vec2 window_center;
  // float window_center_x;
  // float window_center_y;


  double game_time;
  double time;
};

#endif