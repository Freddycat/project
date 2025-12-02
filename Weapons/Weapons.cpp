#include "Weapons.h"
#include "playerCtx.h"
#include "worldCtx.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <iostream>

void Weapon::UpdateWeapon(PlayerCtx &ctx, float delta, vector<WeaponEvent> &events, entt::entity &id)
{
    cooldown -= delta;
    if (cooldown <= 0.0 && ctx.firing)
    {
        cooldown = fire_rate;
        std::cout << "should shoot" << std::endl;
        Weapon::FireWeapon(ctx, events, id);
    }
}
/* old entt
void Weapon::FireWeapon(PlayerCtx &ctx, vector<WeaponEvent> &events, entt::registry &registry)
{
    for (auto weapon : registry.view<GunPart>())
    {
        auto &cmpt = registry.get<GunPart>(weapon);
        cmpt.Shoot(ctx, cmpt, events.projectiles);
    }
    for (auto weapon : registry.view<BeamPart>())
    {
        auto &cmpt = registry.get<BeamPart>(weapon);
        cmpt.Shoot(ctx, cmpt, events.beams);
    }
    for (auto part : registry.view<BlastPart>())
    {
        auto &cmpt = registry.get<BlastPart>(part);
        cmpt.Shoot(ctx, cmpt, events.blasts);
    }
}
    */

void Weapon::FireWeapon(PlayerCtx &ctx, vector<WeaponEvent> &events, entt::entity &id)
{
    vector<WeaponEvent> newEvents;
    vector<WeaponEvent> newBeams;
    vector<WeaponEvent> newProjectiles;

    for (auto &part : parts)
    {
        std::cout << "Storage addr: " << (void *)part.storage
                  << " type: " << part.type << std::endl;
    }

    if (auto *beam = get<BeamPart>())
    {
        std::cout << "foundbeam!" << std::endl;
        std::cout << beam->damage << std::endl;
        std::cout << beam->cooldown << std::endl;
    }

    if (auto *blast = get<BlastPart>())
    {
        std::cout << "foundblast!" << std::endl;
        std::cout << blast->damage << std::endl;
        std::cout << blast->blast_size << std::endl;
    }

    if (auto *test = get<TestComponent>())
    {
        std::cout << "foundtest!" << std::endl;
        std::cout << test->damage << std::endl;
        std::cout << test->size << std::endl;
        std::cout << test->rate << std::endl;
    }

    std::cout << "Blast ID: " << PART_MASK<BlastPart>::value << std::endl;
    std::cout << "Beam  ID: " << PART_MASK<BeamPart>::value << std::endl;
    std::cout << "Test  ID: " << PART_MASK<TestComponent>::value << std::endl;

    for (auto &part : parts)
    {
        switch (part.type)
        {
        case static_cast<uint8_t>(PART_NAME::BEAM):
        {
            WeaponEvent event;
            auto *beam = reinterpret_cast<BeamPart *>(part.storage);
            beam->Shoot(ctx, *beam, event.beams);
            newEvents.push_back(event);
            break;
        }
        case static_cast<uint8_t>(PART_NAME::GUN):
        {
            WeaponEvent event;
            auto *gun = reinterpret_cast<GunPart *>(part.storage);
            gun->Shoot(ctx, *gun, event.projectiles, id);
            newEvents.push_back(event);
            break;
        }
        case static_cast<uint8_t>(PART_NAME::BLAST):
        {
            auto *blast = reinterpret_cast<BlastPart *>(part.storage);

            for (auto &event : newEvents)
            {
                if (blast->affects & PART_MASK<BeamPart>::value)
                {
                    for (auto &b : event.beams)
                    {
                        b.effects |= WorldEffects::EFFECT_EXPLOSIVE;
                        blast->Shoot(ctx, *blast, event.blasts, 1);

                        std::cout << "blast->affects SHOOTING BEAM: Dec=" << blast->affects
                                  << ", Hex=0x" << std::hex << blast->affects << std::dec << std::endl;
                    }
                }

                if (blast->affects & PART_MASK<GunPart>::value)
                {
                    for (auto &p : event.projectiles)
                    {
                        p.effects |= WorldEffects::EFFECT_EXPLOSIVE;
                        blast->Shoot(ctx, *blast, event.blasts, 2);

                        std::cout << "blast->affects SHOOTIN PROJ: Dec=" << blast->affects
                                  << ", Hex=0x" << std::hex << blast->affects << std::dec << std::endl;
                    }
                }
            }

            break;
        }
        }
    }
    for (auto &event : newEvents)
        events.push_back(event);
}

Projectile GunPart::Shoot(PlayerCtx &ctx, GunPart &cmpnt, vector<Projectile> &que, entt::entity &id)
{
    auto &speed = cmpnt.bulletspeed;
    auto &range = cmpnt.maxrange;
    auto &damage = cmpnt.damage;
    auto &start = ctx.pos;
    auto &direction = ctx.facing;
    Projectile pro;
    pro.speed = speed;
    pro.range = range;
    pro.pos = start;
    pro.direction = direction;
    pro.damage = damage;
    // pro.weapon = id;
    que.push_back(pro);
    return pro;
}

void BeamPart::Shoot(PlayerCtx &ctx, BeamPart &beam, vector<Beam> &que)
{
    auto &cooldown = beam.cooldown;
    auto &damage = beam.damage;
    auto &start = ctx.pos;
    auto &end = ctx.xhair;
    Beam bm;
    bm.cooldown = cooldown;
    bm.start = start;
    bm.end = end;
    bm.damage = damage;
    que.push_back(bm);
}

void BlastPart::Shoot(PlayerCtx &ctx, BlastPart &blast, vector<Blast> &que, int i)
{
    std::cout << "int " << i << std::endl;
    auto &size = blast.blast_size;
    auto &rate = blast.blast_rate;
    auto &pos = ctx.xhair;
    Blast b;
    b.max_size = size;
    b.cooldown = rate;
    b.pos = pos;
    que.push_back(b);
}