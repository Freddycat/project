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

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, 800, 600, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_POINT_SMOOTH);

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

//Input input;
//Camera cam;
//Player player;
App app;

bool imgui_on = true;

void initializeWorld()
{
  app.player.posX = 0.0f;
  app.player.posY = 0.0f;
  app.checkSize();
}

auto frame_start = std::chrono::steady_clock::now();
auto frame_end = std::chrono::steady_clock::now();
auto target_frametime = std::chrono::milliseconds(int(1000 / app.target_framerate));
auto next_frametime = std::chrono::steady_clock::now() + target_frametime;

void gameLoop(SDL_Event &event)
{

  frame_start = std::chrono::steady_clock::now();

  app.delta = (frame_start - frame_end);

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

  app.cam.centerCam(app.player); // apply camera offset

  app.input.getMouseWorldPos();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  // ImGui stuff can go in here

  if (imgui_on)
  {
    app.gui.drawWindow();
    app.gui.drawPoints();
  }

  // square

  glColor3f(0.8f, 0.2f, 0.2f); // red (bottom-right)
  glBegin(GL_QUADS);
  glVertex2f(0.0f, 0.0f);     // Top-left
  glVertex2f(800.0f, 0.0f);   // Top-right
  glVertex2f(800.0f, 600.0f); // Bottom-right
  glVertex2f(0.0f, 600.0f);   // Bottom-left
  glEnd();

  glColor3f(0.2f, 0.8f, 0.2f); // green (bottom-left)
  glBegin(GL_QUADS);
  glVertex2f(-800.0f, 0.0f);   // Top-left
  glVertex2f(0.0f, 0.0f);      // Top-right
  glVertex2f(0.0f, 600.0f);    // Bottom-right
  glVertex2f(-800.0f, 600.0f); // Bottom-left
  glEnd();

  glColor3f(0.2f, 0.2f, 0.8f); // blue (top-left)
  glBegin(GL_QUADS);
  glVertex2f(-800.0f, -600.0f); // Top-left
  glVertex2f(0.0f, -600.0f);    // Top-right
  glVertex2f(0.0f, 0.0f);       // Bottom-right
  glVertex2f(-800.0f, 0.0f);    // Bottom-left
  glEnd();

  glColor3f(0.8f, 0.8f, 0.8f); // white (top right)
  glBegin(GL_QUADS);
  glVertex2f(0.0f, -600.0f);   // Top-left
  glVertex2f(800.0f, -600.0f); // Top-right
  glVertex2f(800.0f, 0.0f);    // Bottom-right
  glVertex2f(0.0f, 0.0f);      // Bottom-left
  glEnd();

  app.player.drawPlayer();
  app.player.drawCrosshair();
  app.player.drawBlast();
  

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);

  frameNumber++;

  std::this_thread::sleep_until(next_frametime);
  next_frametime += target_frametime;
  app.game_time += app.delta;
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

  std::cout << "SDL initialized." << std::endl;

  if (!initializeGL())
  {
    std::cerr << "GL initialization failed. Exiting." << std::endl;
    return;
  }

  std::cout << "GL initialized." << std::endl;

  initializeImgui();

  std::cout << "ImGui initialized." << std::endl;

  initializeWorld();

  app.running = true;

  SDL_Event event;
  while (app.running)
  {
    gameLoop(event);
  }
}

int main()
{
  gameStart();
  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}