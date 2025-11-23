#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "app.h"
#include "app_start.h"
#include "shaders.h"
#include "render.h"
#include "global.h"
#include "weapon.h"
#include "world.h"
#include "player.h"
#include "playerCtx.h"
#include "collisions.h"
#include "weaponEventQueue.h"

SDL_Window *window = nullptr;
SDL_GLContext glContext;

bool running = true;

std::string message = "so this is the message";
int number = 42;
int frameNumber = 0;

int target_framerate = 60;
std::chrono::milliseconds target_frametime = std::chrono::milliseconds(int(1000 / target_framerate));
std::chrono::steady_clock::time_point frame_last;
std::chrono::steady_clock::time_point frame_now;
std::chrono::steady_clock::time_point next_frametime;

Input input;
Graphics graphics;
Gizmos gizmos;
Player player;
World world;
Camera camera;
Gui gui;
WeaponManager manager;
Global g;
ColliderCtx colliderCtx;
WorldCtx worldCtx;
PlayerCtx playerCtx;
WeaponEvents wepque;

void gameLoop(SDL_Event &event)
{

    // -- start time stuff --
    frame_now = std::chrono::steady_clock::now();
    g.time_elapsed = std::chrono::duration<float>(frame_now - frame_last).count();
    frame_last = frame_now;
    g.game_time += g.time_elapsed;

    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
        {
            g.console_on = !g.console_on;
        }
    }

    // -- CLEARING (DYNAMIC)--

    gizmos.points.clear();
    gizmos.lines.clear();
    gizmos.line_points.clear();
    gizmos.circles.clear();

    // -- UPDATING --

    input.InputKeyboard(player);
    input.GetMouseInput(playerCtx);
    player.MovePlayer(g.time_elapsed, playerCtx, colliderCtx);
    camera.CenterCam(graphics.vertexID, input, playerCtx);
    input.GetMouseWorldPos(camera, player.cam_center);

    player.UpdateCrosshair(gizmos.points, input.mouse.xhair_pos, playerCtx);
    player.UpdatePlayerDot(gizmos.points, gizmos.capsules);

    UpdateWorldTargets(colliderCtx.collidables, playerCtx, world);

    for (auto &entity : manager.registry.view<Weapon>())
    {
        glm::vec3 newPos = player.pos;
        newPos.z += player.cam_center;
        auto &weapon = manager.registry.get<Weapon>(entity);
        weapon.UpdateWeapon(playerCtx, g.time_elapsed, wepque, manager.registry);
    }

    WorldQueue(wepque, worldCtx);

    worldCtx.UpdateProjectiles(g.time_elapsed, gizmos.points);
    worldCtx.UpdateBlasts(g.time_elapsed, gizmos.circles);
    worldCtx.UpdateLasers(g.time_elapsed, gizmos.lines);
    
    worldCtx.EraseBlasts();
    worldCtx.EraseProjectiles();
    worldCtx.EraseLasers();

    // -- end updating --
    // -- start render --
    render(graphics, camera, gizmos, worldCtx, world, player, gui, manager.registry, player.weapon, input);

    frameNumber++;

    next_frametime = target_frametime + frame_now;
    std::this_thread::sleep_until(next_frametime);
}

std::filesystem::path getExecutableDir()
{
#if defined(_WIN32)
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#else
    // Linux, BSD, etc. using /proc/self/exe
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    buffer[len] = '\0';
    return std::filesystem::path(buffer).parent_path();
#endif
}

void gameStart()
{
    std::cout << message << std::endl;
    std::cout << number << std::endl;

    g.home = getExecutableDir();
    std::cout << "Executable dir: " << g.home << std::endl;

    if (!initializeSDL())
    {
        std::cerr << "SDL initialization failed. Exiting." << std::endl;
        return;
    }

    std::cout << "SDL initialized." << std::endl;

    if (!initializeGL())
    {
        std::cerr << "GL initialization failed. Exiting." << std::endl;
        return;
    }

    std::cout << "GL initialized." << std::endl;

    initializeImgui();

    std::cout << "ImGui initialized." << std::endl;

    CheckSize();

    std::cout << "Checked size." << std::endl;

    SetupShaders(graphics, gizmos, camera);

    world.InitializeWorld(
        player, camera, colliderCtx, world, gizmos,
        graphics.vertexID,
        graphics.vbo_point,
        graphics.vbo_line,
        manager.registry);

    frame_last = std::chrono::steady_clock::now();
    frame_now = frame_last;
    g.game_time = 0.0;
    g.time_elapsed = 0.0;

    running = true;
    g.console_on = true;

    SDL_Event event;
    while (running)
    {
        gameLoop(event);
    }
}

void CheckSize()
{
    SDL_GetWindowSize(g.window, &g.window_width, &g.window_height);

    // Center coordinates
    g.window_center.x = g.window_width / 2.0f;
    g.window_center.y = g.window_height / 2.0f;
}

std::string formatTime()
{
    int total_ms = (int)std::floor(g.game_time * 1000.0);
    int ms = total_ms % 1000;
    int total_seconds = total_ms / 1000;
    int s = total_seconds % 60;
    int total_minutes = total_seconds / 60;
    int m = total_minutes % 60;
    int h = total_minutes / 60;
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d", h, m, s, ms);
    // std::cout << "time done:" << buf << std::endl;
    return std::string(buf);
}

int main()
{
    gameStart();
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}