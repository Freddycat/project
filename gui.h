#ifndef GUI_H
#define GUI_H

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "imgui_internal.h"

struct App;
struct Input;
struct Weapon;
struct Camera;
struct Player;
struct World;

struct Gui
{

  App *app_ptr = nullptr;

  void setApp(App &app)
  {
    app_ptr = &app;
  };

  void drawWindow(Input &input, Camera &camera, World &world, Player &player);
  void mouseInfo(Input &input);
  void camInfo(Camera &cam);
  void playerInfo(Player &player);
  void worldInfo(World &world);
  void weaponInfo(Weapon &weapon);
  void drawPoints();
};

#endif