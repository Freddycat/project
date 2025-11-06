#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>

#include "app.h"

SDL_Window *window = nullptr;

std::string message = "so this is the message";
int number = 42;
int frameNumber = 0;

bool initializeSDL()
{

  std::cout << "Initializing SDL..." << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) != true)
  {
    std::cerr << "Failed to initialize SDL from functiuon: " << SDL_GetError() << std::endl;
    return false;
  }

  std::cout << "Creating SDL window..." << std::endl;

  window = SDL_CreateWindow(
      "Game Window",
      800, 600,
      SDL_WINDOW_OPENGL);

  if (!window)
  {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }
  return true;
}

SDL_GLContext glContext;

bool initializeGL()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

  glContext = SDL_GL_CreateContext(window);

  if (!glContext)
  {
    std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
    return false;
  }

  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD\n";
    return false;
  }

  return true;
}

void initializeImgui()
{
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
  style.WindowRounding = 10.0f;    // Round the window edges
  style.ScrollbarRounding = 10.0f; // Round the scrollbar edges
  ImGui_ImplSDL3_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init("#version 330");
}

// Input input;
// Camera cam;
// Player player;
App app;

bool imgui_on = true;

void gameLoop(SDL_Event &event)
{
  app.frame_now = std::chrono::steady_clock::now();

  app.delta = std::chrono::duration<double>(app.frame_now - app.frame_last).count();

  app.frame_last = app.frame_now;

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
    {
      app.running = false;
    }

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_X)
    {
      imgui_on = !imgui_on;
    }
  }

  app.input.inputKeyboard(app.player);

  app.input.getMouseInput();
  app.player.movePlayer();
  app.cam.centerCam(app.input, app.player);
  app.input.getMouseWorldPos();

  app.game_time += app.delta;
  std::string time = app.formatTime();

  // -- end updating --
  // -- start render --

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();


  if (imgui_on)
  {
    app.gui.drawWindow(app.input, app.cam, app.world, app.player, time);
    app.gui.drawPoints();
  }
  
  app.world.drawWorld();
  app.player.drawCrosshair(app.input);
  app.player.drawPlayer();

  for (auto &weapon : app.player.weapons)
    weapon.updateWeapon(app.input, app.world, app.player, app.delta, weapon.type);

  for (auto &blast : app.world.blasts)
    blast.drawBlast(app.delta);

  for (auto &bullet : app.world.bullets)
    bullet.drawBullet(app.delta);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);

  app.world.eraseBlasts();
  app.world.eraseBullets();

  frameNumber++;

  app.next_frametime = app.target_frametime + app.frame_now;
  std::this_thread::sleep_until(app.next_frametime);
}

void gameStart()
{

  std::cout << message << std::endl;
  std::cout << number << std::endl;

  if (!initializeSDL())
  {
    std::cerr << "SDL initialization failed. Exiting." << std::endl;
    return;
  }

  // SDL_SetWindowRelativeMouseMode(window, true);

  std::cout << "SDL initialized." << std::endl;

  if (!initializeGL())
  {
    std::cerr << "GL initialization failed. Exiting." << std::endl;
    return;
  }

  std::cout << "GL initialized." << std::endl;

  initializeImgui();

  std::cout << "ImGui initialized." << std::endl;

  app.checkSize();

  app.world.initializeWorld();

  app.frame_last = std::chrono::steady_clock::now();
  app.frame_now = app.frame_last;
  app.game_time = 0.0;
  app.delta = 0.0;

  app.running = true;

  SDL_Event event;
  while (app.running)
  {
    gameLoop(event);
  }
}

std::string App::formatTime() const
{
  int total_ms = (int)std::floor(game_time * 1000.0);
  int ms = total_ms % 1000;
  int total_seconds = total_ms / 1000;
  int s = total_seconds % 60;
  int total_minutes = total_seconds / 60;
  int m = total_minutes % 60;
  int h = total_minutes / 60;
  char buf[64];
  std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d", h, m, s, ms);
  //std::cout << "time done:" << buf << std::endl;
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