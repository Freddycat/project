#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <string>

#include "input.h"
#include "GUI.h"
#include "gizmos.h"
#include "camera.h"
#include "global.h"
#include "Graphics.h"

#include <limits.h>

#include <filesystem>

std::string formatTime();

void CheckSize();

#endif