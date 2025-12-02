#ifndef WEAPON_EVENTS_H
#define WEAPON_EVENTS_H

#include "world.h"
#include "partenum.h"

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <entt/entt.hpp>

using std::vector;

struct Blast;
struct Beam;
struct Projectile;
struct PlayerCtx;

struct WeaponEvent
{
    uint8_t id;
    vector<Projectile> projectiles;
    vector<Beam> beams;
    vector<Blast> blasts;
};

struct Weapon
{
    entt::entity id;
    float dmg_multiplier = 1;
    float fire_rate = 0.2f;
    float cooldown = 0.0f;
    void UpdateWeapon(PlayerCtx &ctx, float delta, vector<WeaponEvent> &events, entt::entity &id);
    void FireWeapon(PlayerCtx &ctx, vector<WeaponEvent> &events, entt::entity &id);

    std::vector<Component> parts;

    template <typename T, typename... Args>
    void add(Args &&...args)
    {
        T *ptr = new T(std::forward<Args>(args)...);

        Component part;
        new (&part.storage) T(args...);
        part.destroy = [](void *p)
        { reinterpret_cast<T *>(p)->~T(); };
        part.copy = [](void *dst, const void *src)
        { new (dst) T(*reinterpret_cast<const T *>(src)); };

        part.type = static_cast<uint8_t>(PART_ENUM<T>::value);

        parts.push_back(part);

        std::cout << "[Weapon::add] Adding component type: " << typeid(T).name()
                  << " hash: " << part.type << std::endl;
    }

    template <typename T>
    T *get()
    {
        auto id = PART_MASK<T>::value;
        std::cout << "[Weapon::get] Looking for component type: " << typeid(T).name()
                  << " hash: " << id << std::endl;

        for (auto &part : parts)
            if (part.type == id)
                return reinterpret_cast<T *>(part.storage);
        return nullptr;
    }

    template <typename T>
    void remove()
    {
        auto id = PART_MASK<T>::value;

        for (auto it = parts.begin(); it != parts.end();)
        {
            if (it->type == id)
            {
                it->destroy(it->storage);
                it = parts.erase(it); // remove component
            }
            else
            {
                ++it;
            }
        }
    }

    ~Weapon()
    {
        for (auto &part : parts)
            part.destroy(part.storage);
    }
};

#endif // WEAPON_EVENTS_H