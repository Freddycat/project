#ifndef PLAYER_CTX_H
#define PLAYER_CTX_H

#include <glm/glm.hpp>

using glm::vec3;

struct PlayerCtx
{
    vec3 pos;
    vec3 facing;
    vec3 xhair;
    bool firing = false;
};

#endif // PLAYER_CTX_H