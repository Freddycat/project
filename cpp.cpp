#include <SDL3/SDL.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

std::string message = "so this is the message";
int number = 42;
bool running = false;
int frameNumber = 0;

std::chrono::time_point<std::chrono::steady_clock> frame = std::chrono::steady_clock::now();

SDL_Window *window = nullptr;

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

void gameLoop()
{

  std::cout << message << std::endl;
  std::cout << number << std::endl;

  if (!initializeSDL())
  {
    std::cerr << "SDL initialization failed. Exiting." << std::endl;
    return;
  }

  running = true;

  SDL_Event event;

  while (running == true)

  {

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
    }

    frame += std::chrono::milliseconds(1000);

    SDL_GL_SwapWindow(window);

    std::this_thread::sleep_until(frame);

    std::cout << "Frame: " << frameNumber << std::endl;
    frameNumber++;

    if (frameNumber > 100)
    {
      running = false;
    }
  }
}

int main()
{
  gameLoop();
  return 0;
}