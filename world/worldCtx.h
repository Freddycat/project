#ifndef WORLD_COLLIDABLES_H
#define WORLD_COLLIDABLES_H

#include <glm/glm.hpp>

using glm::vec3;

struct Blast
{
    float max_size;
    float cooldown;
    float time_left;
    vec3 pos;
    float size;
    size_t vertex_start;
    size_t vertex_amount;
};

struct Projectile
{
    vec3 direction;
    vec3 pos;
    float speed;
    float range;
    float distance = 0.0f;
};

struct Laser
{
    vec3 pos_start;
    vec3 pos_end;
    float cooldown;
};

#endif // WORLD_COLLIDABLES_H