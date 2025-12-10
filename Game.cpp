#include <chrono>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_sdl3.h>
#include <string>
#include <thread>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Game.h"
#include "global.h"

SDL_Window *window = nullptr;
SDL_GLContext glContext;

bool running = true;

std::string message = "so this is the message";
int number = 42;
int frameNumber = 0;

int target_framerate = 144;
std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));
std::chrono::steady_clock::time_point next_frametime;

using std::chrono::time_point;

time_point frame_last = std::chrono::steady_clock::now();

void GameLoop(
    SDL_Event &event,
    World &world, WorldCtx &worldCtx, WorldEnv &worldEnv,
    Graphics &graphics, Gizmos &gizmos, Camera &camera, Gui &gui,
    Input &input,
    Player &player, PlayerCtx &playerCtx,
    ColliderCtx &colliderCtx,
    WeaponCtx &weapCtx)
{
    // std::cout << "sanity check" << std::endl;
    //  -- start time stuff --
    time_point frame_now = std::chrono::steady_clock::now();
    float time_elapsed = std::chrono::duration<float>(frame_now - frame_last).count();
    frame_last = frame_now;
    g.game_time += time_elapsed;
    g.time_elapsed = time_elapsed;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
        {
            g.devMenu_on = !g.devMenu_on;
        }
    }

    // -- CLEARING (DYNAMIC)--

    gizmos.points.clear();
    gizmos.lines.clear();
    gizmos.line_points.clear();
    gizmos.circles.clear();
    gizmos.wireframe_cubes.clear();

    // time_point clearAndPoll = std::chrono::steady_clock::now();
    // float clearAndPollTime = std::chrono::duration<float>(clearAndPoll - frame_last).count() * 10.0;
    // std::print("ClearAndPollTime: {}\n", clearAndPollTime);

    // -- UPDATING --

    input.InputKeyboard(player);
    input.GetMouseInput(playerCtx);
    player.MovePlayer(time_elapsed, playerCtx, colliderCtx);
    player.UpdatePlayerCap(playerCtx, gizmos, gizmos.points, gizmos.capsules);
    camera.CenterCam(input, playerCtx);
    input.GetMouseWorldPos(camera, player.shoulder_height.z);
    player.UpdateCrosshair(gizmos.points, input.mouse.xhair_pos, playerCtx);
    /*
        for (auto &entity : manager.registry.view<Weapon>())
        {
            glm::vec3 newPos = player.pos;
            newPos.z += player.cam_center;
            auto &weapon = manager.registry.get<Weapon>(entity);
            weapon.UpdateWeapon(playerCtx, g.time_elapsed, wepque, player.weapon);
        }
     */
    /*
        for (auto &entity : manager.registry.view<WeaponHandle>())
        {

            auto &weaponHandle = manager.registry.get<WeaponHandle>(entity);
            auto &weapon = *weaponHandle.weapon;
            weapon.UpdateWeapon(playerCtx, g.time_elapsed, wepque, player.weapon);
        } */

    if (!g.devMenu_on)
        player.weapons[0].UpdateWeapon(playerCtx, time_elapsed, weapCtx.events, player.weapons[0].id);

    UpdateWorldTargets(colliderCtx.collidables, playerCtx, input, worldCtx);

    // time_point UpdateThings = std::chrono::steady_clock::now();
    // float UpdateThingsTime = std::chrono::duration<float>(UpdateThings - frame_last).count() * 10.0;
    // std::print("UpdateThingsTime {}\n", UpdateThingsTime);

    WorldEvents worldQue;

    WorldCreateQueue(weapCtx.events, worldCtx, colliderCtx.collidables);

    worldCtx.UpdateProjectiles(time_elapsed, gizmos, worldQue, worldEnv, colliderCtx.collidables, player.weapons[0]);
    worldCtx.UpdateBlasts(time_elapsed, gizmos.circles);
    worldCtx.UpdateBeams(time_elapsed, gizmos.lines, worldCtx.beams.list, worldQue, worldEnv, colliderCtx.collidables);

    WorldFXQueue(worldCtx, worldEnv, colliderCtx.collidables);

    WorldHitQueue(worldQue, worldCtx);

    worldCtx.EraseBlasts();
    worldCtx.EraseProjectiles();
    worldCtx.EraseBeams();

    EraseEntt(colliderCtx.collidables);

    WorldClearQueue(weapCtx.events, worldEnv);

    if (g.devMode_on)
    {
        UpdateWorldEdit(colliderCtx.collidables, playerCtx, input, worldCtx, gizmos);
        //std::print("devmode on!\n");
    }

    // time_point QueueThings = std::chrono::steady_clock::now();
    // float QueueThingsTime = std::chrono::duration<float>(QueueThings - frame_last).count() * 10.0;
    // std::print("QueueThingsTime{}\n", QueueThingsTime);

    // -- end updating --
    // -- start render --
    render(time_elapsed, graphics, camera, gizmos, worldCtx, world, player, gui, input, colliderCtx.collidables);

    // time_point RenderThings = std::chrono::steady_clock::now();
    // float RenderThingsTime = std::chrono::duration<float>(RenderThings - frame_last).count() * 10.0;
    // std::print("RenderThingsTime {}\n", RenderThingsTime);

    frameNumber++;

    next_frametime = target_frametime + frame_now;
    std::this_thread::sleep_until(next_frametime);
}

int main()
{
    GameStart();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}