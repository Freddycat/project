
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <print>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>

#include <entt/entt.hpp>

#include "Game.h"
#include "global.h"
#include "gizmos.h"

Global g;

void CheckSize() {
    SDL_GetWindowSize(g.window, &g.window_width, &g.window_height);
    // Center coordinates
    g.window_center.x = g.window_width / 2.0f;
    g.window_center.y = g.window_height / 2.0f;
}

bool initializeSDL() {

    if (SDL_Init(SDL_INIT_VIDEO) != true) {
        std::print("Failed to initialize SDL from functiuon: {}\n", SDL_GetError());
        return false;
    }

    g.window = SDL_CreateWindow(
        "Game Window",
        g.window_width, g.window_height,
        SDL_WINDOW_OPENGL);

    if (!g.window) {
        std::print("Problem with SDL\n");
        SDL_Quit();
        return false;
    }
    std::print("SDL Good!\n");
    return true;
}

bool initializeGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    g.glContext = SDL_GL_CreateContext(g.window);

    if (!g.glContext) {
        std::print("Failed to create OpenGL context: {}\n", SDL_GetError());
        return false;
    }

    if (!gladLoadGL()) {
        std::print("Failed to initialize GLAD\n");
        return false;
    }
    return true;
}

void initializeImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;
    style.GrabRounding = 15.0f;      // Round the "grabber" (thumb)
    style.GrabMinSize = 20.0f;       // Make the grabber bigger
    style.FrameRounding = 10.0f;     // Round the slider background edges
    style.WindowRounding = 10.0f;    // Round the g.window edges
    style.ScrollbarRounding = 10.0f; // Round the scrollbar edges
    ImGui_ImplSDL3_InitForOpenGL(g.window, g.glContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::print("Initialized ImGui\n");
}

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

std::filesystem::path getExecutableDir()

{
#if defined(_WIN32)
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
#else
    // Linux, BSD, etc. using /proc/self/exe
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';
    return std::filesystem::path(buffer).parent_path();
#endif
}

void GameStart() {

    g.home = getExecutableDir();
    std::print("Executable dir: {}", g.home.string());

    if (!initializeSDL()) {
        std::print("SDL initialization failed. Exiting.");
        return;
    }

    std::print("SDL initialized.\n");

    if (!initializeGL()) {
        std::print("GL initialization failed. Exiting.");
        return;
    }

    std::print("GL initialized.\n");

    initializeImgui();

    CheckSize();

    Graphics graphics;
    Gizmos gizmos;
    Gui gui;
    Input input;
    Camera camera;
    Player player;
    PlayerCtx playerCtx;
    ColliderCtx colliderCtx;
    WorldCtx worldCtx;
    WorldEnv worldEnv;
    WeaponCtx weaponCtx;

    SetupShaders(graphics, gizmos, camera);

    // world.InitializeWorld();
    World world(
        player, camera, colliderCtx, worldCtx, world, gizmos);

    g.game_time = 0.0;
    g.time_elapsed = 0.0;

    running = true;

    SDL_Event event;
    while (running) {
        GameLoop(event, world, worldCtx, worldEnv, graphics, gizmos, camera, gui, input, player, playerCtx, colliderCtx, weaponCtx);
    }
}