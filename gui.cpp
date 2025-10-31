#include "gui.h"
#include "app.h"

void Gui::drawWindow()
{
  ImGui::Begin("Yo...");
  ImGui::Text("Sup, bitch");
  ImGui::TreeNode("Sheeit");
  ImGui::Button("Name");
  ImGui::End();
}
void Gui::drawPoints()
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    ImVec2 windowCenter(app_ptr->window_center_x, app_ptr->window_center_y);

    // Convert to screen-space relative to window center
    ImVec2 xhairCenter(
        windowCenter.x + app_ptr->cam.m_camera_delta_x,
        windowCenter.y + app_ptr->cam.m_camera_delta_y
    );

    draw_list->AddText(windowCenter, IM_COL32(255, 255, 255, 255), "Window Center");
    draw_list->AddText(xhairCenter,  IM_COL32(0, 255, 255, 255),   "Crosshair");
}
