#ifndef GUI_H
#define GUI_H

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "imgui_internal.h"

struct App;

struct Gui
{

  App *app_ptr;
  void setApp(App &app)
  {
    app_ptr = &app;
  };

  void drawWindow();
  void drawPoints();
};

#endif