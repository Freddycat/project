#ifndef WEAPON_H
#define WEAPON_H

#include <vector>

using std::vector;

struct WeaponEvents
{
    vector<Projectile> projectiles;
    vector<Laser> lasers;
    vector<Blast> blasts;
};

#endif // WEAPON_H