#include "gui.h"

void drawWindow()
{
    ImGui::Begin("Yo...");
    ImGui::Text("Sup, bitch");
    ImGui::TreeNode("Sheeit");
    ImGui::Button("Name");
    ImGui::End();

    ImGui::Begin("Yo2...");
    ImGui::End();

}