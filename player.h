#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <gizmos.h>

struct Weapon;
struct World;
struct Input;

struct Player
{
  float pos_x = 0.0f;
  float pos_y = 0.0f;
  float velocity_x = 0.0f;
  float velocity_y = 0.0f;
  float speed = 2.0f;

  float height = 48.0f;

  float orientation = -45.0f;

  std::vector<Weapon> weapons;

  Point pos_dot;
  Point xhair_dot;

  Capsule playerCap{
      {0.0, 0.0, 32.0, 0.0},
      {16.0, 16.0, 32.0, 0.0},
      {0.8, 0.8, 0.8, 1.0}};

  void MovePlayer();
  void UpdatePlayerDot(std::vector<Point> &points, std::vector<Capsule> &capsules);
  void UpdateCrosshair(std::vector<Point> &points, glm::vec2 pos);
};

extern Player player;

enum class WeaponType
{
  Deagle,
  Blast,
  Sniper
};

struct Weapon
{
  float blast_size = 150.0f;

  double blast_rate = 0.5f;

  double bullet_live = 0.3f;

  double fire_rate = 0.9f;
  double fire_cooldown = 0.0f;

  WeaponType type;

  void UpdateWeapon(Input &input, double delta, WeaponType type);
  // void DoBlast(Input &input, World &world);
  // void DoBullet(Input &input, Player &player, World &world);
};

#endif