#include "gui.h"
#include "input.h"
#include "camera.h"
#include "graphics.h"
#include "gizmos.h"
#include "player.h"
#include "playerCtx.h"
#include "Weapons.h"
#include "world.h"
#include "worldCtx.h"
#include "global.h"

#include <chrono>
#include <glm/glm.hpp>

ImGuiChildFlags flags = ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders;

void Gui::DrawImGui(
    const std::string time,
    Graphics &graphics, Gizmos &gizmos,
    Player &player, Camera &camera,
    WorldCtx &ctx, World &world,
    Input &input,
    vector<Line> &lines,
    entt::registry &colliders)
{
    ImGui::Begin("Yo...");

    ImGui::Text("Give Up?");

    if (ImGui::Button("Sheeit"))
    {
        running = !running;
    }

    AppTime(time, graphics, gizmos, colliders);
    MouseInfo(input);
    CamInfo(camera);
    PlayerInfo(player, gizmos);
    WorldInfo(ctx);
    WeaponInfo(player);
    GraphicsInfo(graphics);
    DrawScreenInfo(camera, ctx, world, input);
    DrawGameInfo(camera, ctx, lines);
    ImGui::End();
}

void Gui::AppTime(const std::string time, Graphics &graphics, Gizmos &gizmos, entt::registry &colliders)
{
    static float time_accumulator = 0.0;
    static int frames = 0;
    static float fps = 0;

    time_accumulator += g.time_elapsed;
    frames++;

    if (time_accumulator >= 0.2f) // .1 second has passed
    {
        fps = frames / time_accumulator; // averaged fps
        time_accumulator = 0.0f;
        frames = 0;
    }

    if (ImGui::TreeNode("App info"))
    {
        ImGui::BeginChild("app time", ImVec2(0.0f, 0.0f), flags);
        ImGui::Text("FPS:: %f", fps);
        ImGui::Text("Run time: %s", time.c_str());
        ImGui::Text("Points: %d", gizmos.points.size());
        ImGui::Text("Lines: %d", gizmos.lines.size());
        ImGui::Text("Max Lines: %d", graphics.max_lines);
        ImGui::Text("Circles: %d", gizmos.circles.size());
        ImGui::Text("Max Circles: %d", graphics.max_circles);
        ImGui::Text("Projectiles: %d", colliders.view<Projectile>().size());
        ImGui::Text("Beams: %d", colliders.view<Beam>().size());
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

void DrawWeaponGui(Player &player)
{

    auto &weapons = player.weapons;

    for (auto &weapon : weapons)
    {
        static float min = 0.0;
        static float max = 2.0;

        ImGui::Text("Base Weapon");
        ImGui::InputFloat("Rate", &weapon.fire_rate);
        ImGui::SliderScalar("RateSlider", ImGuiDataType_Float, &weapon.fire_rate, &min, &max);
        if (ImGui::CollapsingHeader("Section Name"))
        {
            ImGui::BeginChild("ChildRegion", ImVec2(0, 150), true);
            // Your content here
            ImGui::Text("Inside child");
            ImGui::EndChild();
        }
        for (auto &part : weapon.parts)
        {

            ImGui::Text("Part type: %d", part.type);
            switch (part.type)
            {
            case static_cast<uint8_t>(PART_NAME::GUN):
            {

                static float min = 0.0f;
                static float max = 10000.0f;
                auto *gunPart = reinterpret_cast<GunPart *>(part.storage);

                ImGui::Text("Projectile component");
                ImGui::InputFloat("Speed", &gunPart->bulletspeed);
                ImGui::SliderScalar("SpeedSlider", ImGuiDataType_Float, &gunPart->bulletspeed, &min, &max);
                if (auto *blast = weapon.get<BlastPart>())
                {
                    bool has_blast = (blast->affects & PART_NAME::GUN);
                    if (ImGui::Checkbox("Add Blast##1", &has_blast))
                        blast->affects ^= PART_MASK<GunPart>::value;
                }
                if (ImGui::Button("Remove Projectile"))
                {
                    weapon.remove<GunPart>();
                }
                break;
            }
            case static_cast<uint8_t>(PART_NAME::BEAM):
            {

                static float min = 0.0f;
                static float max = 5.0f;
                auto *beamPart = reinterpret_cast<BeamPart *>(part.storage);
                ImGui::Text("Beam component");
                ImGui::SliderScalar("BeamCooldown", ImGuiDataType_Float, &beamPart->cooldown, &min, &max);
                if (auto *blast = weapon.get<BlastPart>())
                {
                    bool has_blast = (blast->affects & PART_NAME::BEAM);
                    if (ImGui::Checkbox("Add Blast##2", &has_blast))
                        blast->affects ^= PART_MASK<BeamPart>::value;
                }
                if (ImGui::Button("Remove Beam"))
                {
                    weapon.remove<BeamPart>();
                }

                break;
            }
            case static_cast<uint8_t>(PART_NAME::BLAST):
            {

                static float min = 0.0f;
                static float max = 2.0f;
                auto *blastPart = reinterpret_cast<BlastPart *>(part.storage);
                ImGui::Text("Blast component");
                ImGui::InputFloat("BlastRate", &blastPart->blast_rate);
                ImGui::SliderScalar("BlastRateSlider", ImGuiDataType_Float, &blastPart->blast_rate, &min, &max);
                ImGui::SliderFloat("SizeSlider", &blastPart->blast_size, 0, 600);
                if (ImGui::Button("Remove Blast"))
                {
                    weapon.remove<BlastPart>();
                }
            }
            break;
            }
        }
        if (!weapon.get<GunPart>())
        {
            if (ImGui::Button("Add Gun"))
            {
                weapon.add<GunPart>();
            }
        }

        if (!weapon.get<BlastPart>())
        {
            if (ImGui::Button("Add Blast"))
            {
                weapon.add<BlastPart>();
            }
        }

        if (!weapon.get<BeamPart>())
        {
            if (ImGui::Button("Add Beam"))
            {
                weapon.add<BeamPart>();
            }
        }
    }
}

void Gui::WeaponInfo(Player &player)
{
    static bool is_weapon_info_on;
    if (ImGui::Button("Weapon info"))
        is_weapon_info_on = !is_weapon_info_on;

    if (is_weapon_info_on)
    {
        ImGui::Begin("Weapon info");

        DrawWeaponGui(player);

        ImGui::End();
    }
}

void DrawGraphicsGui(Graphics &graphics)
{
    static float noiseFrequency = 1.0f; // lower = more zoomed-out
    static float noiseSpeedX = 0.1f;      // 0 = no scrolling
    static float noiseSpeedY = 0.1f;      // 0 = no scrolling
    static float gradientScale = 1.0f;   // how strong the sway direction reacts

    ImGui::SliderFloat("Noise Frequency", &noiseFrequency, 0.001f, 4.0f);
    ImGui::SliderFloat("Noise Speed X", &noiseSpeedX, 0.0f, 2.0f);
    ImGui::SliderFloat("Noise Speed Y", &noiseSpeedY, 0.0f, 2.0f);
    ImGui::SliderFloat("Gradient Scale", &gradientScale, 0.0001f, 2.0f);

    Gfx::Use(graphics.shaders[SHADER_GRASS]);

    glUniform1f(glGetUniformLocation(graphics.shaders[SHADER_GRASS], "noise_frequency"), noiseFrequency);
    glUniform1f(glGetUniformLocation(graphics.shaders[SHADER_GRASS], "noise_speed_x"), noiseSpeedX);
    glUniform1f(glGetUniformLocation(graphics.shaders[SHADER_GRASS], "noise_speed_y"), noiseSpeedY);
    glUniform1f(glGetUniformLocation(graphics.shaders[SHADER_GRASS], "gradient_scale"), gradientScale);

    //Gfx::UnbindVAO();
}

void Gui::GraphicsInfo(Graphics &graphics)
{
    static bool is_graphics_info_on;
    if (ImGui::Button("Graphics info"))
        is_graphics_info_on = !is_graphics_info_on;

    if (is_graphics_info_on)
    {
        ImGui::Begin("Graphics info");

        DrawGraphicsGui(graphics);

        ImGui::End();
    }
}

void Gui::WorldInfo(WorldCtx &world)
{
    auto &blasts = world.blasts.list;
    auto &beams = world.beams.list;
    auto &projectiles = world.projectiles.list;

    if (ImGui::TreeNode("World info"))
    {
        ImGui::BeginChild("World info", ImVec2(0.0f, 0.0f), flags);
        ImGui::Text("blasts: %d", (int)blasts.size());
        for (size_t i = 0; i < blasts.size(); i++)
        {
            ImGui::Text("Blast %d - Pos: (%.1f, %.1f, %.1f) Time: %.2f", (int)i,
                        blasts[i].pos.x,
                        blasts[i].pos.y,
                        blasts[i].pos.z,
                        blasts[i].cooldown);
        }
        for (size_t i = 0; i < beams.size(); i++)
        {
            ImGui::Text("Beam %d - Time: %.2f", (int)i,
                        blasts[i].cooldown);
        }
        for (size_t i = 0; i < projectiles.size(); i++)
        {
            ImGui::Text("Projectile %d - dist: %.2f", (int)i,
                        projectiles[i].distance);
        }
        // need to pass multiple or somehing
        // ImGui::Text("Cells: %d", (int)world.cells.size());

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

void Gui::DrawScreenInfo(Camera &camera, WorldCtx &ctx, World &world, Input &input)
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

        draw_list->AddText(ImPos, IM_COL32(255, 255, 255, 100), buf);
    }
}

uint8_t mapTo255(float x, float max_number)
{
    if (max_number <= 0)
        return 0;
    float t = x / max_number;
    t = std::clamp(t, 0.0f, 1.0f);
    return static_cast<uint8_t>(t * 255.0f);
}

void Gui::DrawGameInfo(Camera &camera, WorldCtx &world, vector<Line> &lines)
{
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();

    for (auto &target : world.targets)
    {

        if (!target.show_info)
            continue;

        float pos_z = target.pos.z + target.size * 1.4;

        glm::vec4 clip = camera.projection * camera.view * glm::vec4(target.pos.x, target.pos.y, pos_z, 1.0f);
        glm::vec3 ndc = glm::vec3(clip) / clip.w;
        ImVec2 ImPos(
            (ndc.x * 0.5f + 0.5f) * g.window_width,
            (1.0f - (ndc.y * 0.5f + 0.5f)) * g.window_height);
        char buf[64];
        snprintf(buf, sizeof(buf), "HP: %.1f", target.hp);
        draw_list->AddText(ImPos, IM_COL32(255, 255, 255, 240), buf);
        /*
        float red = mapTo255(target.hp, target.max_hp);
        float green = mapTo255(target.hp, target.max_hp);
        */
        float hp_pos = target.pos.z + target.size;
        float ratio = target.hp / target.max_hp; // 0.0 to 1.0
        float red = 1.0f - ratio;
        float green = ratio;

        float max_pos = 16;

        vec4 color = {red, green, 0, 1};

        Point a, b;
        a.pos = {target.pos.x + max_pos * ratio,
                 target.pos.y - max_pos * ratio,
                 hp_pos};

        b.pos = {target.pos.x - max_pos * ratio,
                 target.pos.y + max_pos * ratio,
                 hp_pos};
        a.color = color;
        b.color = color;
        Line line{a, b};
        lines.push_back(line);
    }
}
