#ifndef GLOBAL_H
#define GLOBAL_H

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <filesystem>

extern bool running;

struct Global
{
    std::filesystem::path home;
    SDL_Window *window;
    SDL_GLContext glContext;

    glm::vec2 window_size;
    int window_width;
    int window_height;

    glm::vec2 window_center;

    float game_time;
    float time_elapsed;

    bool console_on = true;
};

extern Global g;

#endif