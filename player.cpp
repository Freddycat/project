#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <chrono>
#include <glm/glm.hpp>
#include "player.h"
#include "app.h"
#include "world.h"

#include <iostream>

void Player::movePlayer()
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

void Player::drawPlayer()
{
  glColor3f(0.6f, 0.0f, 0.6f);
  glPointSize(8.0f);
  glBegin(GL_POINTS);
  glVertex2f(pos_x, pos_y);
  glEnd();
}

void Player::drawCrosshair(Input &input)
{

  glColor3f(1.0f, 0.0f, 0.0f);
  glPointSize(4.0f);
  glBegin(GL_POINTS);
  glVertex2f(input.mouse_world_pos_x,
             input.mouse_world_pos_y);
  glEnd();
}

void Weapon::updateWeapon(Input &input, World &world, Player &player, double delta, WeaponType type)
{
  fire_cooldown -= delta;

  if (fire_cooldown <= 0.0)
  {
    fire_cooldown = fire_rate;
    switch (type)
    {
    case WeaponType::Deagle:
      doBullet(input, player, world);
      break;
    case WeaponType::Blast:
      doBlast(input, world);
      break;
    }
    std::cout << "Weapon fired" << std::endl;
  }
}

void Weapon::doBlast(Input &input, World &world)
{
  Blast blast;
  blast.time = blast_rate;
  blast.rate = blast_rate;
  blast.pos_x = input.mouse_world_pos_x;
  blast.pos_y = input.mouse_world_pos_y;
  blast.radius = blast_radius;
  world.blasts.push_back(blast);
}

void Weapon::doBullet(Input &input, Player &player, World &world)
{
  Bullet bullet;
  bullet.time = bullet_live;
  bullet.start_pos_x = player.pos_x;
  bullet.start_pos_y = player.pos_y;
  bullet.pos_x = input.mouse_world_pos_x;
  bullet.pos_y = input.mouse_world_pos_y;
  world.bullets.push_back(bullet);
}
