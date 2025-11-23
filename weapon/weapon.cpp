#include "weapon.h"
#include "playerCtx.h"
#include "worldCtx.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <iostream>

void Weapon::UpdateWeapon(PlayerCtx &ctx, float delta, WeaponEvents &events, entt::registry &weapreg)
{
    cooldown -= delta;
    if (cooldown <= 0.0 && ctx.firing)
    {
        cooldown = fire_rate;
        std::cout << "should shoot" << std::endl;
        Weapon::FireWeapon(ctx, events, weapreg);
    }
}

void Weapon::FireWeapon(PlayerCtx &ctx, WeaponEvents &events, entt::registry &registry)
{
    for (auto &weapon : registry.view<ProjectileComponent>())
    {
        auto &cmpt = registry.get<ProjectileComponent>(weapon);
        cmpt.Shoot(ctx, cmpt, events.projectiles);
    }
    for (auto &weapon : registry.view<LaserComponent>())
    {
        auto &cmpt = registry.get<LaserComponent>(weapon);
        cmpt.Shoot(ctx, cmpt, events.lasers);
    }
    for (auto &weapon : registry.view<BlastComponent>())
    {
        auto &cmpt = registry.get<BlastComponent>(weapon);
        cmpt.Shoot(ctx, cmpt, events.blasts);
    }
}

void ProjectileComponent::Shoot(PlayerCtx &ctx, ProjectileComponent &component, vector<Projectile> &que)
{
    auto &speed = component.bulletspeed;
    auto &range = component.maxrange;
    auto &start = ctx.pos;
    auto &direction = ctx.facing;
    Projectile p;
    p.speed = speed;
    p.range = range;
    p.pos = start;
    p.direction = direction;
    que.push_back(p);
    //std::cout << "shooted bullit" << std::endl;
}

void LaserComponent::Shoot(PlayerCtx &ctx, LaserComponent &laser, vector<Laser> &que)
{
    auto &cooldown = laser.cooldown;
    auto &start = ctx.pos;
    auto &end = ctx.xhair;
    Laser l;
    l.cooldown = cooldown;
    l.pos_start = start;
    l.pos_end = end;
    que.push_back(l);
}

void BlastComponent::Shoot(PlayerCtx &ctx, BlastComponent &blast, vector<Blast> &que)
{
    auto &size = blast.blast_size;
    auto &rate = blast.blast_rate;
    auto &pos = ctx.xhair;
    Blast b;
    b.max_size = size;
    b.cooldown = rate;
    b.pos = pos;
    que.push_back(b);
    //std::cout << "shooted blast: " << glm::to_string(b.pos) << std::endl;
}