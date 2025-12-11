#ifndef WORLD_COLLIDERS_H
#define WORLD_COLLIDERS_H

#include <glm/glm.hpp>

using glm::vec3;

enum class WorldEffects : uint8_t
{
    NONE = 0,
    EFFECT_EXPLOSIVE = 1 << 0,
};


const std::unordered_map<WorldEffects, const char*> WorldEffectsNames{
    {WorldEffects::NONE, "NONE"},
    {WorldEffects::EFFECT_EXPLOSIVE, "EFFECT_EXPLOSIVE"},
};

// Setters must return WorldEffects

inline WorldEffects operator|(WorldEffects lhs, WorldEffects rhs)
{
  return static_cast<WorldEffects>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline WorldEffects &operator|=(WorldEffects &lhs, WorldEffects rhs)
{
  // This now works because lhs | rhs returns WorldEffects
  lhs = lhs | rhs; 
  return lhs;
}

// Checker must return the underlying integer type for implicit 'if' conversion

inline uint8_t operator&(WorldEffects lhs, WorldEffects rhs)
{
    // Return uint8_t (the underlying type) for use in 'if' statements
    return (static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}
struct Hit
{
    entt::entity target;
    entt::entity weapon;
    float damage;
};

struct Blast
{
    float damage;
    float max_size;
    float cooldown;
    float time_left;
    vec3 pos;
    float size;
    uint8_t id;
};

struct Projectile
{
    float damage;
    vec3 direction;
    vec3 pos;
    float speed;
    float range;
    float distance = 0.0f;
    bool remove = false;
    WorldEffects effects = WorldEffects::NONE;
};

struct Beam
{
    entt::entity weapon;
    float damage;
    vec3 start;
    vec3 end;
    float cooldown;
    bool hit = false;
    bool remove = false;
    WorldEffects effects = WorldEffects::NONE;
};

#endif // WORLD_COLLIDERS_H