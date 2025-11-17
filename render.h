#define RENDER_H
#ifdef RENDER_H

#include <entt/entt.hpp>

struct Graphics;
struct Gizmos;
struct Camera;
struct Input;
struct Player;
struct World;
struct Gui;

void render(Graphics &graphics, Camera &camera, Gizmos &gizmos, World &world, Player &player, Gui &gui, entt::registry &registry, entt::entity &weapon, Input &input);

#endif // RENDER_H