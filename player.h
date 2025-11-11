#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <gizmos.h>

struct App;
struct Input;
struct World;
struct Blast;

struct Weapon;

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

  void MovePlayer();
  void DrawPlayer(std::vector<Point> &points);
  void DrawCrosshair(std::vector<Point> &points, glm::vec2 pos);
};

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

  void UpdateWeapon(Input &input, World &world, Player &player, double delta, WeaponType type, std::vector<Point> &lines, std::vector<Circle> &circle);
  //void DoBlast(Input &input, World &world);
  //void DoBullet(Input &input, Player &player, World &world);
};

#endif