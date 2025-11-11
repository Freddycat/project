#ifndef GUI_H
#define GUI_H

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include "imgui_internal.h"

struct App;
struct Input;
struct Weapon;
struct Camera;
struct Graphics;
struct Gizmos;
struct Player;
struct World;
struct State;

struct Gui
{
  void DrawWindow(State &state, Graphics &graphics, Gizmos &gizmos, Input &input, Camera &camera, World &world, Player &player, std::string &time);
  void AppTime(std::string &time, Graphics &graphics, Gizmos &gizmos);
  void MouseInfo(Input &input);
  void CamInfo(Camera &cam);
  void PlayerInfo(Player &player);
  void WorldInfo(World &world);
  void WeaponInfo(Weapon &weapon);
  void DrawList(State &state, Input &input, World &world, Camera &cam);
};

#endif