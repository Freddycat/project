#ifndef WEAPON_H
#define WEAPON_H

#include "world.h"
#include <glm/glm.hpp>
#include <vector>
#include <entt/entt.hpp>

struct Blast;
struct Laser;

struct WeaponManager
{
    entt::registry registry;
};

struct LaserComponent
{
    double cooldown = 0.3f;
    void Shoot(glm::vec3 target, glm::vec3 start, double delta, std::vector<Laser> &lasers);
};

struct BlastComponent
{
    float blast_size = 150.0f;
    double blast_rate = 0.5f;
    void Shoot(glm::vec3 target, glm::vec3 start, double delta, std::vector<Blast> &blasts);
};

struct Weapon
{
    entt::entity id;
    double fire_rate = 0.9f;
    double cooldown = 0.0f;
    void UpdateWeapon(glm::vec3 target, glm::vec3 start, double delta, std::vector<Blast> &blasts, std::vector<Laser> &lasers, entt::registry &registry);
    void FireWeapon(glm::vec3 target, glm::vec3 start, double delta, std::vector<Blast> &blasts, std::vector<Laser> &lasers, entt::registry &registry);
};

using WeaponComponents = entt::type_list<Weapon, LaserComponent, BlastComponent>;

#endif // WEAPON_H