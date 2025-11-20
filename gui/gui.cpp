#include "gui.h"
#include "input.h"
#include "camera.h"
#include "graphics.h"
#include "gizmos.h"
#include "player.h"
#include "weapon.h"
#include "world.h"
#include "global.h"

#include <glm/glm.hpp>

ImGuiChildFlags flags = ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders;

void Gui::DrawImGui(const std::string time, Graphics &graphics, Gizmos &gizmos, Player &player, Camera &camera, World &world, entt::registry &registry, entt::entity &weapon, Input &input)
{
    ImGui::Begin("Yo...");

    ImGui::Text("Give Up?");

    if (ImGui::Button("Sheeit"))
    {
        running = !running;
    }

    AppTime(time, graphics, gizmos);
    MouseInfo(input);
    CamInfo(camera);
    PlayerInfo(player, gizmos);
    WorldInfo(world);
    WeaponInfo(player, registry, weapon);
    DrawScreenInfo(camera, world, input);
    ImGui::End();
}

void Gui::AppTime(const std::string time, Graphics &graphics, Gizmos &gizmos)
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

void Gui::MouseInfo(Input &input)
{
    auto &mouse = input.mouse;
    if (ImGui::TreeNode("Mouse info"))
    {
        ImGui::BeginChild("mouse info", ImVec2(0.0f, 0.0f), flags);
        ImGui::Text("Screen Pos: (%.1f, %.1f)", mouse.screen_pos.x, mouse.screen_pos.y);
        ImGui::Text("World Pos: (%.1f, %.1f, %.1f)", mouse.world_pos.x, mouse.world_pos.y, mouse.world_pos.z);
        ImGui::Text("Camera Pos: (%.1f, %.1f, %.1f)", mouse.xhair_pos.x, mouse.xhair_pos.y, mouse.xhair_pos.z);
        ImGui::EndChild();
        ImGui::TreePop();
    }
}

void Gui::CamInfo(Camera &camera)
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

// START WEAPON STUFF

void DrawWeaponGui(entt::registry &registry, entt::entity &weapon)
{

    if (registry.all_of<Weapon>(weapon))
    {
        auto &component = registry.get<Weapon>(weapon);

        static double min = 0.0;
        static double max = 2.0;

        ImGui::Text("Base Weapon");
        ImGui::InputDouble("Rate", &component.fire_rate);
        ImGui::SliderScalar("RateSlider", ImGuiDataType_Double, &component.fire_rate, &min, &max);
    }

    if (registry.all_of<BlastComponent>(weapon))
    {
        auto &component = registry.get<BlastComponent>(weapon);

        static double min = 0.0f;
        static double max = 2.0f;
        ImGui::Text("Blast component");
        ImGui::InputDouble("BlastRate", &component.blast_rate);
        ImGui::SliderScalar("BlastRateSlider", ImGuiDataType_Double, &component.blast_rate, &min, &max);
        ImGui::SliderFloat("SizeSlider", &component.blast_size, 0, 600);
        if (ImGui::Button("Remove Blast"))
        {
            registry.remove<BlastComponent>(weapon);
        }
    }
    if (registry.all_of<LaserComponent>(weapon))
    {
        auto &component = registry.get<LaserComponent>(weapon);

        static double min = 0.0f;
        static double max = 5.0f;
        ImGui::Text("Laser component");
        ImGui::SliderScalar("LaserCooldown", ImGuiDataType_Double, &component.cooldown, &min, &max);
        if (ImGui::Button("Remove Laser"))
        {
            registry.remove<LaserComponent>(weapon);
        }
    }

    if (!registry.all_of<BlastComponent>(weapon))
    {
        if (ImGui::Button("Add Blast"))
        {
            registry.emplace<BlastComponent>(weapon);
        }
    }

    if (!registry.all_of<LaserComponent>(weapon))
    {
        if (ImGui::Button("Add Laser"))
        {
            registry.emplace<LaserComponent>(weapon);
        }
    }
}

void Gui::WeaponInfo(Player &player, entt::registry &registry, entt::entity &weapon)
{
    static bool is_weapon_info_on;
    if (ImGui::Button("Weapon info"))
        is_weapon_info_on = !is_weapon_info_on;

    if (is_weapon_info_on)
    {
        ImGui::Begin("Weapon info");

        DrawWeaponGui(registry, weapon);

        ImGui::End();
    }
}

void Gui::WorldInfo(World &world)
{
    if (ImGui::TreeNode("World info"))
    {
        ImGui::BeginChild("World info", ImVec2(0.0f, 0.0f), flags);
        ImGui::Text("blasts: %d", (int)world.blasts.size());
        for (size_t i = 0; i < world.blasts.size(); i++)
        {
            ImGui::Text("Blast %d - Pos: (%.1f, %.1f, %.1f) Time: %.2f", (int)i,
                        world.blasts[i].pos.x,
                        world.blasts[i].pos.y,
                        world.blasts[i].pos.z,
                        world.blasts[i].cooldown);
        }
        for (size_t i = 0; i < world.lasers.size(); i++)
        {
            ImGui::Text("Laser %d - Time: %.2f", (int)i,
                        world.blasts[i].cooldown);
        }
        ImGui::Text("Cells: %d", (int)world.cells.size());

        ImGui::EndChild();
        ImGui::TreePop();
    }
}

void Gui::PlayerInfo(Player &player, Gizmos &gizmos)
{
    if (ImGui::TreeNode("Player info"))
    {
        ImGui::BeginChild("Player info", ImVec2(0.0f, 0.0f), flags);
        ImGui::Text("Player Pos: (%.1f, %.1f)", player.pos.x, player.pos.y);
        ImGui::Text("Player Accel: (%.1f)", player.accel);
        ImGui::Text("Player Vel: (%.1f, %.1f)", player.velocity.x, player.velocity.y);
        ImGui::Text("Player WishDir: (%.1f, %.1f)", player.wish_dir.x, player.wish_dir.y);
        ImGui::Text("Player Speed: (%.1f)", player.speed);

        float height = gizmos.capsules[0].size.z;
        if (ImGui::InputFloat("Height", &height))
        {
            gizmos.capsules[0].size.z = height;
        }

        ImGui::EndChild();
        ImGui::TreePop();
    }
}

glm::vec2 GetWorldPos(glm::vec2 &pos, Camera &camera)
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

void Gui::DrawScreenInfo(Camera &camera, World &world, Input &input)
{
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

    auto &mouse = input.mouse;

    ImVec2 window_center(g.window_center.x, g.window_center.y);
    ImVec2 screen_pos(mouse.screen_pos.x, mouse.screen_pos.y);

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
