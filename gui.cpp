#include "gui.h"
#include "input.h"
#include "camera.h"
#include "graphics.h"
#include "gizmos.h"
#include "player.h"
#include "world.h"
#include "global.h"
#include <glm/glm.hpp>

Gui gui;

ImGuiChildFlags flags = ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border;

void Gui::DrawWindow(const std::string time)
{
  ImGui::Begin("Yo...");

  ImGui::Text("Give Up?");

  if (ImGui::Button("Sheeit"))
  {
    running = !running;
  }

  AppTime(time);
  MouseInfo();
  CamInfo();
  WeaponInfo();
  PlayerInfo();
  WorldInfo();
  ImGui::End();
}

void Gui::AppTime(const std::string time)
{
  if (ImGui::TreeNode("App info"))
  {
    ImGui::BeginChild("app time", ImVec2(0.0f, 0.0f), flags);
    ImGui::Text("Run time: %s", time.c_str());
    ImGui::Text("Points: %d", gizmos.points.size());
    ImGui::Text("Lines: %d", gizmos.lines.size());
    ImGui::Text("Max Lines: %d", graphics.max_lines);
    ImGui::Text("Circles: %d", gizmos.circles.size());
    ImGui::Text("Max Circles: %d", graphics.max_circles);
    ImGui::EndChild();
    ImGui::TreePop();
  }
}

void Gui::MouseInfo()
{
  Input *input = Input::Instance();
  glm::vec2 mouse_screen;
  mouse_screen = input->GetMouse().screen_pos;
  glm::vec2 mouse_world;
  mouse_world = input->GetMouse().world_pos;
  if (ImGui::TreeNode("Mouse info"))
  {
    ImGui::BeginChild("mouse info", ImVec2(0.0f, 0.0f), flags);
    ImGui::Text("Screen Pos: (%.1f, %.1f)", mouse_screen.x, mouse_screen.y);
    ImGui::Text("World Pos: (%.1f, %.1f)", mouse_world.x, mouse_world.y);
    ImGui::EndChild();
    ImGui::TreePop();
  }
}

void Gui::CamInfo()
{
  if (ImGui::TreeNode("Camera info"))
  {
    ImGui::BeginChild("cam info", ImVec2(0.0f, 0.0f), flags);

    float offset[3] = {camera.offset.x, camera.offset.y, camera.offset.z};
    if (ImGui::InputFloat3("offset", offset))
    {
      camera.offset.x = offset[0];
      camera.offset.y = offset[1];
      camera.offset.z = offset[2];
    }

    float pos[3] = {camera.position.x, camera.position.y, camera.position.z};
    if (ImGui::InputFloat3("Pos", pos))
    {
      camera.position.x = pos[0];
      camera.position.y = pos[1];
      camera.position.z = pos[2];
    }

    float target[3] = {camera.target.x, camera.target.y, camera.target.z};
    if (ImGui::InputFloat3("Target", target))
    {
      camera.target.x = target[0];
      camera.target.y = target[1];
      camera.target.z = target[2];
    }

    ImGui::EndChild();
    ImGui::TreePop();
  }
}

void Gui::WeaponInfo()
{
  if (ImGui::TreeNode("Weapon info"))
  {
    ImGui::BeginChild("Weapon info", ImVec2(0.0f, 0.0f), flags);

    ImGui::Text("Cooldown: %.1f", player.weapons[0].fire_cooldown);
    ImGui::EndChild();
    ImGui::TreePop();
  }
}

void Gui::WorldInfo()
{
  if (ImGui::TreeNode("World info"))
  {
    ImGui::BeginChild("World info", ImVec2(0.0f, 0.0f), flags);
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

    ImGui::EndChild();
    ImGui::TreePop();
  }
}

void Gui::PlayerInfo()
{
  if (ImGui::TreeNode("Player info"))
  {
    ImGui::BeginChild("Player info", ImVec2(0.0f, 0.0f), flags);
    ImGui::Text("Player Pos: (%.1f, %.1f)", player.pos_x, player.pos_y);

    float height = gizmos.capsules[0].size.z;
    if (ImGui::InputFloat("Height", &height))
    {
      gizmos.capsules[0].size.z = height;
    }

    ImGui::EndChild();
    ImGui::TreePop();
  }
}

glm::vec2 GetWorldPos(glm::vec2 &pos)
{
  float ndc_x = (2.0f * pos.x) / g.window_width - 1.0f;
  float ndc_y = 1.0f - (2.0f * pos.y) / g.window_height;

  glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);
  glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f);

  glm::vec4 ray_near_world = camera.inverse_view * ray_clip;
  glm::vec4 ray_far_world = camera.inverse_view * ray_far_clip;
  ray_near_world /= ray_near_world.w;
  ray_far_world /= ray_far_world.w;

  glm::vec3 origin = glm::vec3(ray_near_world);
  glm::vec3 dir = glm::normalize(glm::vec3(ray_far_world - ray_near_world));
  float t = -origin.z / dir.z;
  glm::vec3 world_pos = origin + t * dir;
  return world_pos;
}

void Gui::DrawList()
{
  ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

  Input *input = Input::Instance();
  glm::vec2 mouse_screen;
  mouse_screen = input->GetMouse().screen_pos;

  ImVec2 window_center(g.window_center.x, g.window_center.y);
  ImVec2 screen_pos(mouse_screen.x, mouse_screen.y);

  draw_list->AddText(window_center, IM_COL32(255, 255, 255, 255), "Window Center");
  draw_list->AddText(screen_pos, IM_COL32(0, 255, 255, 255), "Screen pos");
  for (auto &cell : world.cells)
  {
    glm::vec4 clip = camera.projection * camera.view * glm::vec4(cell.pos.x, cell.pos.y, 0.0f, 1.0f);
    glm::vec3 ndc = glm::vec3(clip) / clip.w;
    ImVec2 ImPos(
        (ndc.x * 0.5f + 0.5f) * g.window_width,
        (1.0f - (ndc.y * 0.5f + 0.5f)) * g.window_height);

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", cell.id);

    draw_list->AddText(ImPos, IM_COL32(255, 255, 255, 255), buf);
  }
}
