#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

struct App;
struct Input;
struct World;
struct Blast;
struct Player;

struct Weapon;

struct Player
{
  float pos_x = 0.0f;
  float pos_y = 0.0f;
  float velocity_x = 0.0f;
  float velocity_y = 0.0f;
  float speed = 2.5f;
  float orientation = -45.0f;

  std::vector<Weapon> weapons;

  void MovePlayer();
  void DrawPlayer();
  void DrawCrosshair(Input &input);
};

enum class WeaponType
{
  Deagle,
  Blast,
  Sniper
};

struct Weapon
{
  float blast_radius = 15.0f;

  double blast_rate = 0.5f;
  double blast_time = 0.0f;

  double bullet_live = 0.3f;
  double bullet_time = 0.0f;

  double fire_rate = 0.9f;
  double fire_cooldown = 0.0f;

  WeaponType type;

  void UpdateWeapon(Input &input, World &world, Player &player, double delta, WeaponType type);
  void DoBlast(Input &input, World &world);
  void DoBullet(Input &input, Player &player, World &world);
};

#endif