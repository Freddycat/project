#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <glm/glm.hpp>
#include "player.h"
#include "app.h"
#include "world.h"

#include <iostream>

void Player::MovePlayer()
{

  float theta = glm::radians(orientation); // -45 degrees

  // Rotate velocity vector by orientation
  float rotated_vx = velocity_x * cos(theta) - velocity_y * sin(theta);
  float rotated_vy = velocity_x * sin(theta) + velocity_y * cos(theta);

  pos_x += rotated_vx;
  pos_y += rotated_vy;

  // Apply friction to gradually stop the player
  velocity_x *= 0.8f; // Friction factor for x
  velocity_y *= 0.8f; // Friction factor for y

  // If velocity is very small, set it to zero to prevent drifting
  if (std::abs(velocity_x) < 0.1f)
    velocity_x = 0.0f;
  if (std::abs(velocity_y) < 0.1f)
    velocity_y = 0.0f;
}

void Player::DrawPlayer(std::vector<Point> &points)
{
  points[0].pos = {pos_x, pos_y, 0.0f};
  points[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
  /*
 pos_dot.pos = {pos_x, pos_y, 0.0};
 pos_dot.color = glm::vec3(1.0f, 1.0f, 1.0f); */
}

void Player::DrawCrosshair(std::vector<Point> &points, glm::vec2 pos)
{

  points[1].pos = {pos.x, pos.y, 0.0f};
  points[1].color = glm::vec3(1.0f, 1.0f, 1.0f);
  /*
 xhair_dot.pos = glm::vec3(pos.x, pos.y, 0.0f);
 xhair_dot.color = glm::vec3(1.0f, 1.0f, 1.0f); */
}

void Weapon::UpdateWeapon(Input &input, World &world, Player &player, double delta, WeaponType type, std::vector<Point> &lines, std::vector<Circle> &circles)
{
  fire_cooldown -= delta;

  if (fire_cooldown <= 0.0)
  {
    fire_cooldown = fire_rate;
    switch (type)
    {
    case WeaponType::Deagle:
      DoBullet(input, player, world);
      break;
    case WeaponType::Blast:
      Weapon blast = player.weapons[0];
      BlastManager::CreateBlast(blast.blast_size, blast.blast_rate, input.mouse_world_pos, world.blasts, circles);
      //DoBlast(input, world);
      break;
    }
    // std::cout << "Weapon fired" << std::endl;
  }
}
/* 
void Weapon::DoBlast(Input &input, World &world)
{
  Blast blast;
  blast.cooldown = blast_rate;
  blast.rate = blast_rate;
  blast.pos.x = input.mouse_world_pos.x;
  blast.pos.y = input.mouse_world_pos.y;
  blast.pos.z = 0.0f;
  blast.radius = blast_radius;
  world.blasts.push_back(blast);
} 
*/

void Weapon::DoBullet(Input &input, Player &player, World &world)
{
  Bullet bullet;
  bullet.time = bullet_live;
  bullet.start_pos_x = player.pos_x;
  bullet.start_pos_y = player.pos_y;
  bullet.pos_x = input.mouse_world_pos.x;
  bullet.pos_y = input.mouse_world_pos.y;
  world.bullets.push_back(bullet);
}
