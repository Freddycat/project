#ifndef GUI_H
#define GUI_H

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <entt/entt.hpp>

struct Input;
struct Player;
struct Gizmos;
struct Graphics;
struct Camera;
struct World;
struct WorldCtx;

struct Gui
{
    void DrawImGui(const std::string time, Graphics &graphics, Gizmos &gizmos, Player &player, Camera &camera, WorldCtx &ctx, World &world, entt::registry &registry, entt::entity &weapon, Input &input);
    void AppTime(const std::string time, Graphics &graphics, Gizmos &gizmos);
    void MouseInfo(Input &input);
    void CamInfo(Camera &camera);
    void PlayerInfo(Player &player, Gizmos &gizmos);
    void WorldInfo(WorldCtx &world);
    void WeaponInfo(Player &player, entt::registry &registry, entt::entity &weapon);
    void DrawScreenInfo(Camera &camera, WorldCtx &ctx, World &world, Input &input);
    void DrawGameInfo(Camera &camera, World &world);
};

#endif