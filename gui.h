#ifndef GUI_H
#define GUI_H

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

struct Input;

struct Gui
{
  void DrawWindow(const std::string time);
  void AppTime(const std::string time);
  void MouseInfo();
  void CamInfo();
  void PlayerInfo();
  void WorldInfo();
  void WeaponInfo();
  void DrawList();
};

extern Gui gui;

#endif