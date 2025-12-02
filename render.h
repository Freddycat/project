#define RENDER_H
#ifdef RENDER_H

#include <entt/entt.hpp>

struct Graphics;
struct Gizmos;
struct Camera;
struct Input;
struct Player;
struct World;
struct WorldCtx;
struct Gui;

void render(float delta, Graphics &graphics, Camera &camera, Gizmos &gizmos, WorldCtx &ctx, World &world, Player &player, Gui &gui, Input &input,entt::registry &colliders);

#endif // RENDER_H