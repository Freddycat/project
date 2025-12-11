#ifndef WEAPON_PARTS_H
#define WEAPON_PARTS_H

//#include "World.h"
#include <iostream>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>

using std::vector;

struct Blast;
struct Beam;
struct Projectile;
struct PlayerCtx;

struct GunPart {
    entt::entity id;
    float damage = 8;
    float bulletspeed = 1000.0f;
    float maxrange = 10000.0f;
    glm::vec3 direction;
    Projectile Shoot(PlayerCtx &ctx, GunPart &bullet, vector<Projectile> &que, entt::entity &id);
    uint8_t type;
};

struct BeamPart {
    entt::entity id;
    float damage = 5;
    float cooldown = 0.3f;
    void Shoot(PlayerCtx &ctx, BeamPart &beam, vector<Beam> &que);
    uint8_t type;
};

struct BlastPart {
    // entt::entity root;
    // entt::entity id;
    float damage = 4;
    float blast_size = 150.0f;
    float blast_rate = 0.5f;
    void Shoot(PlayerCtx &ctx, BlastPart &blast, vector<Blast> &que, int i);
    uint8_t type;
    uint8_t affects;
};

struct TestComponent {
    float damage = 4;
    float size = 150.0f;
    float rate = 0.5f;
};

constexpr size_t MAX_WEAPON_SIZE =
    std::max({sizeof(GunPart),
              sizeof(BeamPart),
              sizeof(BlastPart)});

constexpr size_t MAX_COMPONENT_ALIGN =
    std::max({alignof(BeamPart),
              alignof(GunPart),
              alignof(BlastPart),
              alignof(TestComponent)});

struct Component {
    alignas(MAX_COMPONENT_ALIGN) unsigned char storage[MAX_WEAPON_SIZE];
    //alignas(max_align_t) char storage[MAX_WEAPON_SIZE];
    void (*destroy)(void *);            // how to free it
    void (*copy)(void *, const void *); // optional, for cloning weapons
    uint8_t type;                       // your own type ID enum
};

#endif // WEAPON_EVENTS_H