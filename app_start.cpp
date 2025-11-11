#include "app_start.h"
#include "app.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

bool initializeSDL(App &app)
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


bool initializeGL(App &app, SDL_GLContext &glContext, SDL_Window *window)
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

void initializeImgui(SDL_GLContext &glContext)
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
