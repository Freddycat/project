#ifndef GLOBAL_H
#define GLOBAL_H

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <filesystem>

extern bool running;

struct Global
{
    float set_width = 1280;
    float set_height = 720;

    int window_width = 1280;
    int window_height = 720;

    std::filesystem::path home;
    SDL_Window *window;
    SDL_GLContext glContext;

    glm::vec2 window_size;

    glm::vec2 window_center;

    float game_time;
    float time_elapsed;

    bool devMenu_on = true;
    bool devMode_on = false;
    bool edit_shiftMod = false;
    bool enable_grass = true;
};

extern Global g;

#endif