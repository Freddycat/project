#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "app.h"

App app;

auto &time_elapsed = app.state.time;
auto &game_time = app.state.game_time;
auto &state = app.state;

SDL_Window *window = nullptr;

bool imgui_on = true;

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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4.6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4.6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

  // shader programs
  app.graphics.vertexID = glGraphics::Shader::InitializeShader("vert.glsl", "frag.glsl");
  app.graphics.circleID = glGraphics::Shader::InitializeShader("circle.glsl", "frag.glsl");

  if (glIsProgram(app.graphics.vertexID))
    glUseProgram(app.graphics.vertexID);
  else
    std::cerr << "Invalid shader program\n";

  glGraphics::checkGLError("Graphics::Shader::use");

  GLuint projLocation = glGetUniformLocation(app.graphics.vertexID, "uProjection");
  if (projLocation == -1)
    std::cerr << "Warning: uProjection uniform not found!\n";

  GLuint viewLocation = glGetUniformLocation(app.graphics.vertexID, "uView");
  if (viewLocation == -1)
    std::cerr << "Warning: uView uniform not found!\n";

  glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(app.camera.projection));

  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  app.graphics.points.reserve(1000);
  app.graphics.lines.reserve(1000);
  app.gizmos.circles.reserve(50);

  app.graphics.max_points = std::max<size_t>(256, app.graphics.points.capacity());
  app.graphics.max_lines = std::max<size_t>(1024, app.graphics.lines.capacity());
  app.graphics.max_circles = std::max<size_t>(64, app.gizmos.circles.capacity());

  // app.graphics.max_points = app.graphics.lines.size() * 10000;
  // app.graphics.max_lines = app.graphics.lines.size() * 10000;
  // app.graphics.max_circles = app.gizmos.circles.size() * 50;

  // points
  glGenVertexArrays(1, &app.graphics.vao_point);
  glBindVertexArray(app.graphics.vao_point);
  glGenBuffers(1, &app.graphics.vbo_point);

  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_point);
  glBufferData(GL_ARRAY_BUFFER, app.graphics.max_points * sizeof(Point), app.graphics.points.data(), GL_DYNAMIC_DRAW);

  // pos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, pos));
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, color));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  // lines
  glGenVertexArrays(1, &app.graphics.vao_line);
  glBindVertexArray(app.graphics.vao_line);
  glGenBuffers(1, &app.graphics.vbo_line);

  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_line);
  glBufferData(GL_ARRAY_BUFFER, app.graphics.max_lines * sizeof(Point), app.graphics.lines.data(), GL_DYNAMIC_DRAW);

  // pos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, pos));
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, color));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  if (glIsProgram(app.graphics.circleID))
    glUseProgram(app.graphics.circleID);
  else
    std::cerr << "Invalid shader program\n";

  glGraphics::checkGLError("Graphics::Shader::use");

  // circles (static)

  GLuint projLocationCirc = glGetUniformLocation(app.graphics.circleID, "uProjection");
  if (projLocationCirc == -1)
    std::cerr << "Warning: uProjection uniform not found!\n";

  GLuint viewLocationCirc = glGetUniformLocation(app.graphics.circleID, "uView");
  if (viewLocationCirc == -1)
    std::cerr << "Warning: uView uniform not found!\n";

  glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(app.camera.projection));

  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  int segments = 50;
  float radius = 1.0f;
  for (int i = 0; i < segments; ++i)
  {
    float theta = (2.0f * M_PI * i) / segments;
    app.graphics.base_circle.push_back(glm::vec3(cos(theta), sin(theta), 0.0f));
  }

  glGenVertexArrays(1, &app.graphics.vao_circle);
  glBindVertexArray(app.graphics.vao_circle);

  glGenBuffers(1, &app.graphics.vbo_circle_buf);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_circle_buf);
  glBufferData(GL_ARRAY_BUFFER, app.graphics.base_circle.size() * sizeof(glm::vec3), app.graphics.base_circle.data(), GL_STATIC_DRAW);

  // pos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
  glEnableVertexAttribArray(0);

  // centered (instance)
  glGenBuffers(1, &app.graphics.vbo_circles);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_circles);
  glBufferData(GL_ARRAY_BUFFER, app.graphics.max_circles * sizeof(Circle), nullptr, GL_DYNAMIC_DRAW);

  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, color));
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  // center
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, center));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1); // center changes per instance

  // scale
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Circle), (void *)offsetof(Circle, size));
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1); // scale changes per instance

  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(1.0f);

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

void gameLoop(SDL_Event &event)
{
  app.frame_now = std::chrono::steady_clock::now();

  time_elapsed = std::chrono::duration<double>(app.frame_now - app.frame_last).count();

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

  app.input.InputKeyboard(app.player);

  app.input.GetMouseInput();
  app.player.MovePlayer();
  app.camera.CenterCam(app.graphics.vertexID, app.input, app.player);
  app.input.GetMouseWorldPos(state, app.camera);

  game_time += time_elapsed;
  std::string time_str = app.formatTime();

  // -- end updating --
  // -- start render --

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  if (imgui_on)
  {

    app.gui.DrawWindow(state, app.graphics, app.gizmos, app.input, app.camera, app.world, app.player, time_str);

    app.gui.DrawList(state, app.input, app.world, app.camera);
  }
  // app.world.DrawWorld(app.lines);
  // app.world.DrawHouse();

  // clear circles
  app.gizmos.circles.clear();

  app.player.DrawCrosshair(app.graphics.points, app.input.mouse_world_pos);
  app.player.DrawPlayer(app.graphics.points);

  for (auto &weapon : app.player.weapons)
    weapon.UpdateWeapon(app.input, app.world, app.player, time_elapsed, weapon.type, app.graphics.lines, app.gizmos.circles);

  BlastManager::UpdateBlasts(time_elapsed, app.world.blasts, app.gizmos.circles);

  for (auto &bullet : app.world.bullets)
    bullet.DrawBullet(time_elapsed);

  glUseProgram(app.graphics.vertexID);

  if (app.graphics.points.size() > app.graphics.max_points)
  {
    app.graphics.max_points = app.graphics.points.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_points * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
  }

  if (app.graphics.lines.size() > app.graphics.max_lines)
  {
    app.graphics.max_lines = app.graphics.lines.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_lines * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);
  }

  if (app.gizmos.circles.size() > app.graphics.max_circles)
  {
    app.graphics.max_circles = app.gizmos.circles.size() * 1.25f;
    glBufferData(GL_ARRAY_BUFFER, app.graphics.max_circles * sizeof(Circle), nullptr, GL_DYNAMIC_DRAW);
  }

  GLuint projLocation = glGetUniformLocation(app.graphics.vertexID, "uProjection");
  GLuint viewLocation = glGetUniformLocation(app.graphics.vertexID, "uView");
  glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(app.camera.projection));
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  glBindVertexArray(app.graphics.vao_point);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_point);

  glBufferSubData(GL_ARRAY_BUFFER, 0, app.graphics.points.size() * sizeof(Point), app.graphics.points.data());
  glPointSize(6.0f);

  glDrawArrays(GL_POINTS, 0, app.graphics.points.size());

  glBindVertexArray(app.graphics.vao_line);
  glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_line);

  glBufferSubData(GL_ARRAY_BUFFER, 0, app.graphics.lines.size() * sizeof(Point), app.graphics.lines.data());
  glLineWidth(2.0f);

  glDrawArrays(GL_LINES, 0, app.graphics.lines.size());

  glBindVertexArray(0);

  // circle render
  glUseProgram(app.graphics.circleID);
  // camera stuff
  GLuint projLocationCirc = glGetUniformLocation(app.graphics.circleID, "uProjection");
  GLuint viewLocationCirc = glGetUniformLocation(app.graphics.circleID, "uView");
  glUniformMatrix4fv(projLocationCirc, 1, GL_FALSE, glm::value_ptr(app.camera.projection));
  glUniformMatrix4fv(viewLocationCirc, 1, GL_FALSE, glm::value_ptr(app.camera.view));

  glBindVertexArray(app.graphics.vao_circle);

  if (!app.gizmos.circles.empty())
  {
    glBindBuffer(GL_ARRAY_BUFFER, app.graphics.vbo_circles);

    glBufferSubData(GL_ARRAY_BUFFER, 0, app.gizmos.circles.size() * sizeof(Circle), app.gizmos.circles.data());
  }

  glDrawArraysInstanced(GL_LINE_LOOP, 0, app.graphics.base_circle.size(), app.gizmos.circles.size());

  glBindVertexArray(0);

  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
    std::cerr << "GL error: " << err << std::endl;

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window);

  app.world.EraseBlasts(app.graphics.lines);
  app.world.EraseBullets();

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

  app.CheckSize();

  std::cout << "Checked size." << std::endl;

  app.world.InitializeWorld(
      app.player, app.camera,
      app.graphics.points, app.graphics.lines,
      app.graphics.vertexID,
      app.graphics.vbo_point,
      app.graphics.vbo_line);

  std::cout << "Initialized world." << std::endl;

  app.frame_last = std::chrono::steady_clock::now();
  app.frame_now = app.frame_last;
  game_time = 0.0;
  time_elapsed = 0.0;

  app.state.running = true;

  SDL_Event event;
  while (app.state.running)
  {
    gameLoop(event);
  }
}

void App::CheckSize()
{
  SDL_GetWindowSize(window, &state.window_width, &state.window_height);

  // Center coordinates
  state.window_center.x = state.window_width / 2.0f;
  state.window_center.y = state.window_height / 2.0f;

  camera.window_center = state.window_center;
  input.window_center = state.window_center;
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