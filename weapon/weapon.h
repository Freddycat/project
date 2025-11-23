#ifndef WEAPON_EVENTS_H
#define WEAPON_EVENTS_H

#include "world.h"
#include "weaponEventQueue.h"

#include <glm/glm.hpp>
#include <vector>
#include <entt/entt.hpp>

using std::vector;

struct Blast;
struct Laser;
struct Projectile;
struct PlayerCtx;

struct WeaponManager
{
    entt::registry registry;
};

struct ProjectileComponent
{
    float bulletspeed = 1000.0f;
    float maxrange = 10000.0f;
    glm::vec3 direction;
    void Shoot(PlayerCtx &ctx, ProjectileComponent &bullet, vector<Projectile> &que);
};

struct LaserComponent
{
    float cooldown = 0.3f;
    void Shoot(PlayerCtx &ctx, LaserComponent &laser, vector<Laser> &que);
};

struct BlastComponent
{
    float blast_size = 150.0f;
    float blast_rate = 0.5f;
    void Shoot(PlayerCtx &ctx, BlastComponent &blast, vector<Blast> &que);
};

struct Weapon
{
    entt::entity id;
    float fire_rate = 0.9f;
    float cooldown = 0.0f;
    void UpdateWeapon(PlayerCtx &ctx, float delta, WeaponEvents &events, entt::registry &weapreg);
    void FireWeapon(PlayerCtx &ctx, WeaponEvents &events, entt::registry &weapreg);
};

using WeaponComponents = entt::type_list<Weapon, LaserComponent, BlastComponent>;

#endif // WEAPON_EVENTS_H