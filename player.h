#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

struct App;
struct Input;
struct World;
struct Blast;

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
  double fire_rate = 0.9f;
  double fire_cooldown = 0.0f;
  void updateWeapon(Input &input, World &world, double delta, WeaponType type);
  void doBlast(Input &input, World &world);
};

struct Player
{

  App *app_ptr = nullptr;

  float pos_x = 0.0f;
  float pos_y = 0.0f;
  float speed = 6.0f;

  std::vector<Weapon> weapons;

  void drawPlayer();
  void drawCrosshair(Input &input);

  void setApp(App &app)
  {
    app_ptr = &app;
  }
};

#endif