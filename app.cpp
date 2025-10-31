#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>

#include "app.h"

#include "input.h"

#include "gui.h"

SDL_Window *window = nullptr;

std::string message = "so this is the message";
int number = 42;
bool running = false;
int frameNumber = 0;

std::chrono::time_point<std::chrono::steady_clock> frame = std::chrono::steady_clock::now();

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

App app;
Input input;
Dot dot;

// Dot dot2;

// Dot dot3;

// Dot dot4;

// std::vector<Dot> dots;

bool imgui_on = true;

void initializeWorld()
{

  dot.posX = 0.0f;
  dot.posY = 0.0f;
  app.checkSize();

  /*
  dot2.posX = 380.0f;
  dot2.posY = 300.0f;
  dot3.posX = 420.0f;
  dot3.posY = 300.0f;
  dot4.posX = 400.0f;
  dot4.posY = 320.0f;

  dots.push_back(dot1);
  dots.push_back(dot2);
  dots.push_back(dot3);
  dots.push_back(dot4); */
}

// Call once per frame before drawing anything
void applyCamera()
{

  float zoom_amount = 0.3f;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float midPoint_x = dot.posX + input.m_world_pos_x * zoom_amount;
  float midPoint_y = dot.posY + input.m_world_pos_y * zoom_amount;

  app.cam_point_x = midPoint_x - app.window_center_x;
  app.cam_point_y = midPoint_y - app.window_center_y;

  glTranslatef(-app.cam_point_x, -app.cam_point_y, 0); // move the "view"
}

/*
// Update camera position based on input
void updateCamera(float dx, float dy) {
    cameraX += dx;
    cameraY += dy;
} */

void drawBlast()
{
  int segments = 32;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINE_LOOP);

  for (int i = 0; i < segments; i++)
  {
    float theta = 2.0f * 3.1415926f * float(i) / float(segments);
    float x = dot.weapon.blast_size * 10.0f * cosf(theta);
    float y = dot.weapon.blast_size * 10.0f * sinf(theta);
    glVertex2f(input.m_screen_pos_x + app.cam_point_x + x,
               input.m_screen_pos_y + app.cam_point_y + y);
  }

  glEnd();
}

void gameLoop()
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

  running = true;

  SDL_Event event;

  while (running == true)

  {

    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }

      if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_X)
      {
        imgui_on = !imgui_on;
      }
    }

    input.inputKeyboard(dot);
    input.inputMouse(app);

    applyCamera(); // apply camera offset

    // frame += std::chrono::milliseconds(1000);
    SDL_Delay(16);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGui stuff can go in here

    if (imgui_on)
    {
      drawWindow();
    }

    // square

    /*     glColor3f(1.0f, 0.8f, 1.0f); // White
        glBegin(GL_QUADS);
        glVertex2f(100.0f, 50.0f);  // Top-left
        glVertex2f(700.0f, 50.0f);  // Top-right
        glVertex2f(700.0f, 550.0f); // Bottom-right
        glVertex2f(100.0f, 550.0f); // Bottom-left
        glEnd();
    */
    glColor3f(0.8f, 0.0f, 0.0f); // purp(bottom-right)
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);     // Top-left
    glVertex2f(800.0f, 0.0f);   // Top-right
    glVertex2f(800.0f, 600.0f); // Bottom-right
    glVertex2f(0.0f, 600.0f);   // Bottom-left
    glEnd();

    glColor3f(0.0f, 0.8f, 0.0f); // green (bottom-left)
    glBegin(GL_QUADS);
    glVertex2f(-800.0f, 0.0f);   // Top-left
    glVertex2f(0.0f, 0.0f);      // Top-right
    glVertex2f(0.0f, 600.0f);    // Bottom-right
    glVertex2f(-800.0f, 600.0f); // Bottom-left
    glEnd();

    glColor3f(0.0f, 0.0f, 0.8f); // blue (top-left)
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

    // updateCamera();
    dot.drawDot();

    drawBlast();

    // dot.checkEdge();
    /*
        for (auto &dot : dots)
        {
          // dot->draw();
          // dot->checkEdge();
        } */

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);

    frameNumber++;

    if (frameNumber > 10000)
    {
      running = false;
    }
  }
}

int main()
{
  gameLoop();

  SDL_GL_DestroyContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}