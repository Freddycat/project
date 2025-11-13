#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <string>

#include "input.h"
#include "gui.h"
#include "gizmos.h"
#include "camera.h"
#include "world.h"
#include "player.h"
#include "global.h"
#include "graphics.h"

std::string formatTime();

void CheckSize();

#endif