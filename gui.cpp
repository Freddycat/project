#include "gui.h"
#include "input.h"
#include "camera.h"
#include "graphics.h"
#include "player.h"
#include "world.h"
#include "app_state.h"
#include <glm/glm.hpp>

ImGuiChildFlags flags = ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border;

void Gui::DrawWindow(State &state, Graphics &graphics, Gizmos &gizmos, Input &input, Camera &camera, World &world, Player &player, std::string &time)
{
  ImGui::Begin("Yo...");

  ImGui::Text("Give Up?");

  if (ImGui::Button("Sheeit"))
  {
    state.running = !state.running;
  }

  AppTime(time, graphics, gizmos);
  MouseInfo(input);
  CamInfo(camera);
  WeaponInfo(player.weapons[0]);
  PlayerInfo(player);
  WorldInfo(world);

  ImGui::End();
}

void Gui::AppTime(std::string &time, Graphics &graphics, Gizmos &gizmos)
{
  ImGui::BeginChild("app time", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Time:");
  ImGui::Indent();
  ImGui::Text("Run time: %s", time.c_str());
  ImGui::Text("Points: %d", graphics.points.size());
  ImGui::Text("Lines: %d", graphics.lines.size());
  ImGui::Text("Max Lines: %d", graphics.max_lines);
  ImGui::Text("Circles: %d", gizmos.circles.size());
  ImGui::Text("Max Circles: %d", graphics.max_circles);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::MouseInfo(Input &input)
{
  ImGui::BeginChild("mouse info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Mouse Info:");
  ImGui::Indent();
  ImGui::Text("Screen Pos: (%.1f, %.1f)", input.mouse_screen_pos.x, input.mouse_screen_pos.y);
  ImGui::Text("World Pos: (%.1f, %.1f)", input.mouse_world_pos.x, input.mouse_world_pos.y);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::CamInfo(Camera &cam)
{
  ImGui::BeginChild("cam info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Camera Info:");
  ImGui::Indent();

  
  float offset[3] = {cam.offset.x, cam.offset.y, cam.offset.z};
  if (ImGui::InputFloat3("offset", offset))
  {
    cam.offset.x = offset[0];
    cam.offset.y = offset[1];
    cam.offset.z = offset[2];
  }
  

  float pos[3] = {cam.position.x, cam.position.y, cam.position.z};
  if (ImGui::InputFloat3("Pos", pos))
  {
    cam.position.x = pos[0];
    cam.position.y = pos[1];
    cam.position.z = pos[2];
  }

  float target[3] = {cam.target.x, cam.target.y, cam.target.z};
  if (ImGui::InputFloat3("Target", target))
  {
    cam.target.x = target[0];
    cam.target.y = target[1];
    cam.target.z = target[2];
  }

  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::WeaponInfo(Weapon &weapon)
{
  ImGui::BeginChild("Weapon info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Weapon Info:");
  ImGui::Indent();
  ImGui::Text("Cooldown: %.1f", weapon.fire_cooldown);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::WorldInfo(World &world)
{
  ImGui::BeginChild("World info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("World Info:");
  ImGui::Indent();
  ImGui::Text("blasts: %d", (int)world.blasts.size());
  for (size_t i = 0; i < world.blasts.size(); i++)
  {
    ImGui::Text(" Blast %d - Pos: (%.1f, %.1f, %.1f) Time: %.2f", (int)i,
                world.blasts[i].pos.x,
                world.blasts[i].pos.y,
                world.blasts[i].pos.z,
                world.blasts[i].cooldown);
  }
  ImGui::Text("Cells: %d", (int)world.cells.size());
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::PlayerInfo(Player &player)
{
  ImGui::BeginChild("Player info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Player Info:");
  ImGui::Indent();
  ImGui::Text("Player Pos: (%.1f, %.1f)", player.pos_x, player.pos_y);
  ImGui::Unindent();
  ImGui::EndChild();
}

glm::vec2 GetWorldPos(State &state, Camera &cam, glm::vec2 &pos)
{
  float ndc_x = (2.0f * pos.x) / state.window_width - 1.0f;
  float ndc_y = 1.0f - (2.0f * pos.y) / state.window_height;

  glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);
  glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f);

  glm::vec4 ray_near_world = cam.inverse_view * ray_clip;
  glm::vec4 ray_far_world = cam.inverse_view * ray_far_clip;
  ray_near_world /= ray_near_world.w;
  ray_far_world /= ray_far_world.w;

  glm::vec3 origin = glm::vec3(ray_near_world);
  glm::vec3 dir = glm::normalize(glm::vec3(ray_far_world - ray_near_world));
  float t = -origin.z / dir.z;
  glm::vec3 world_pos = origin + t * dir;
  return world_pos;
}

void Gui::DrawList(State &state, Input &input, World &world, Camera &cam)
{
  ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

  ImVec2 window_center(state.window_center.x, state.window_center.y);
  ImVec2 screen_pos(input.mouse_screen_pos.x, input.mouse_screen_pos.y);

  draw_list->AddText(window_center, IM_COL32(255, 255, 255, 255), "Window Center");
  draw_list->AddText(screen_pos, IM_COL32(0, 255, 255, 255), "Screen pos");
  for (auto &cell : world.cells)
  {
    glm::vec4 clip = cam.projection * cam.view * glm::vec4(cell.pos.x, cell.pos.y, 0.0f, 1.0f);
    glm::vec3 ndc = glm::vec3(clip) / clip.w;
    ImVec2 ImPos(
        (ndc.x * 0.5f + 0.5f) * state.window_width,
        (1.0f - (ndc.y * 0.5f + 0.5f)) * state.window_height);

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", cell.id);

    draw_list->AddText(ImPos, IM_COL32(255, 255, 255, 255), buf);
  }
}
