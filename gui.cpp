#include "gui.h"
#include "app.h"

ImGuiChildFlags flags = ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border;

void Gui::drawWindow(Input &input, Camera &camera, World &world, Player &player)
{
  ImGui::Begin("Yo...");

  ImGui::Text("Give Up?");

  if (ImGui::Button("Sheeit"))
  {
    app_ptr->running = !app_ptr->running;
  }

  mouseInfo(input);
  camInfo(camera);
  weaponInfo(player.weapons[0]);
  playerInfo(player);
  worldInfo(world);

  ImGui::End();
}

void Gui::mouseInfo(Input &input)
{
  ImGui::BeginChild("mouse info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Mouse Info:");
  ImGui::Indent();
  ImGui::Text("Screen Pos: (%.1f, %.1f)", input.mouse_screen_pos_x, input.mouse_screen_pos_y);
  ImGui::Text("World Pos: (%.1f, %.1f)", input.mouse_world_pos_x, input.mouse_world_pos_y);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::camInfo(Camera &cam)
{
  ImGui::BeginChild("cam info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Camera Info:");
  ImGui::Indent();
  ImGui::Text("Camera Origin: (%.1f, %.1f)", cam.origin_x, cam.origin_y);
  ImGui::Text("Camera Center: (%.1f, %.1f)", cam.center_x, cam.center_y);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::weaponInfo(Weapon &weapon)
{
  ImGui::BeginChild("Weapon info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Weapon Info:");
  ImGui::Indent();
  ImGui::Text("Cooldown: %.1f", weapon.fire_cooldown);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::worldInfo(World &world)
{
  ImGui::BeginChild("World info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("World Info:");
  ImGui::Indent();
  ImGui::Text("blasts: %d", (int)world.blasts.size());
  for (size_t i = 0; i < world.blasts.size(); i++)
  {
    ImGui::Text(" Blast %d - Pos: (%.1f, %.1f) Time: %.2f", (int)i,
                world.blasts[i].pos_x,
                world.blasts[i].pos_y,
                world.blasts[i].time);
  }
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::playerInfo(Player &player)
{
  ImGui::BeginChild("Player info", ImVec2(0.0f, 0.0f), flags);
  ImGui::Text("Player Info:");
  ImGui::Indent();
  ImGui::Text("Player Pos: (%.1f, %.1f)", player.pos_x, player.pos_y);
  ImGui::Unindent();
  ImGui::EndChild();
}

void Gui::drawPoints()
{
  ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

  ImVec2 windowCenter(app_ptr->window_center_x, app_ptr->window_center_y);

  ImVec2 xhairCenter(
      app_ptr->input.mouse_screen_pos_x,
      app_ptr->input.mouse_screen_pos_y);

  draw_list->AddText(windowCenter, IM_COL32(255, 255, 255, 255), "Window Center");
  draw_list->AddText(xhairCenter, IM_COL32(0, 255, 255, 255), "Crosshair");
}
