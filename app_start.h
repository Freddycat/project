#ifndef APP_START_H
#define APP_START_H

#include <SDL3/SDL.h>

struct App;

bool initializeSDL(App &app);
bool initializeGL(App &app, SDL_GLContext &glContext, SDL_Window *window);
void initializeImgui(SDL_GLContext &glContext);


#endif