#ifndef PLAYER_H
#define PLAYER_H

struct App;

struct Player
{

  App *app_ptr = nullptr;

  float posX = 0.0f;
  float posY = 0.0f;
  float speed = 6.0f;

  struct Weapon
  {
    float blast_size = 15.0f;
    float blast_speed = 6.0f;
    float rate = 9.0f;
  } weapon;

  void drawPlayer();
  void drawBlast();
  void drawCrosshair();

  void setApp(App &app)
  {
    app_ptr = &app;
  }
};

#endif