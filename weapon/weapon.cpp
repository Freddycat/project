#include "weapon.h"
#include <iostream>

void Weapon::UpdateWeapon(glm::vec3 target, glm::vec3 start, float delta, std::vector<Bullet> &bullets, std::vector<Blast> &blasts, std::vector<Laser> &lasers, entt::registry &registry)
{
    cooldown -= delta;

    // std::cout << "cooldown" << cooldown << "delta" << delta << std::endl;

    if (cooldown <= 0.0)
    {
        cooldown = fire_rate;
        std::cout << "should shoot" << std::endl;
        Weapon::FireWeapon(target, start, delta, bullets, blasts, lasers, registry);
    }
}

void Weapon::FireWeapon(glm::vec3 target, glm::vec3 start, float delta, std::vector<Bullet> &bullets, std::vector<Blast> &blasts, std::vector<Laser> &lasers, entt::registry &registry)
{
    for (auto &weapon : registry.view<BulletComponent>())
    {
        auto &bullet = registry.get<BulletComponent>(weapon);
        bullet.Shoot(target, start, bullet.bulletspeed, bullet.maxrange, bullets);
    }
    for (auto &weapon : registry.view<LaserComponent>())
    {
        auto &laser = registry.get<LaserComponent>(weapon);
        laser.Shoot(target, start, delta, lasers);
    }
    for (auto &weapon : registry.view<BlastComponent>())
    {
        auto &blast = registry.get<BlastComponent>(weapon);
        blast.Shoot(target, start, delta, blasts);
    }
}

void BulletComponent::Shoot(glm::vec3 target, glm::vec3 start, float speed, float range, std::vector<Bullet> &bullet)
{
    CreateBullet(target, start, speed, range, bullet);
}

void BlastComponent::Shoot(glm::vec3 target, glm::vec3 start, float delta, std::vector<Blast> &blasts)
{
    CreateBlast(blast_size, blast_rate, target, blasts);

    std::cout << "shooted" << std::endl;
}

void LaserComponent::Shoot(glm::vec3 target, glm::vec3 start, float delta, std::vector<Laser> &lasers)
{
    CreateLaser(start, target, cooldown, lasers);

    std::cout << "shooted" << std::endl;
}