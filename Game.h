#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <string>

#include "Graphics.h"
#include "GUI.h"
#include "shaders.h"
#include "Render.h"
#include "input.h"
#include "Weapons.h"
#include "camera.h"
#include "World.h"
#include "player.h"
#include "playerCtx.h"
#include "collisions.h"


std::string formatTime();
void CheckSize();
bool initializeSDL();
bool initializeGL();
void initializeImgui();

void GameStart();
void GameLoop(
    SDL_Event &event,
    World &world, WorldCtx &worldCtx, WorldEnv &worldEnv,
    Graphics &graphics, Gizmos &gizmos, Camera &camera, Gui &gui,
    Input &input,
    Player &player, PlayerCtx &playerCtx,
    ColliderCtx &colliderCtx,
    WeaponCtx &weapCtx);

#endif