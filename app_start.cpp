
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "app_start.h"
#include "shaders.h"
#include "global.h"

bool initializeSDL()
{
  std::cout << "Initializing SDL..." << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) != true)
  {
    std::cerr << "Failed to initialize SDL from functiuon: " << SDL_GetError() << std::endl;
    return false;
  }

  std::cout << "Creating SDL window..." << std::endl;

  g.window = SDL_CreateWindow(
      "Game Window",
      g.window_width, g.window_height,
      SDL_WINDOW_OPENGL);

  if (!g.window)
  {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }
  return true;
}


bool initializeGL()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4.6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4.6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  g.glContext = SDL_GL_CreateContext(g.window);

  if (!g.glContext)
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
  style.WindowRounding = 10.0f;    // Round the g.window edges
  style.ScrollbarRounding = 10.0f; // Round the scrollbar edges
  ImGui_ImplSDL3_InitForOpenGL(g.window, g.glContext);
  ImGui_ImplOpenGL3_Init("#version 330");
}
