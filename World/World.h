#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <gizmos.h>
#include <entt/entt.hpp>

#include "worldCtx.h"

struct App;
struct Blast;
struct Projectile;
struct Beam;
struct Player;
struct Input;
struct Camera;
struct Cell;
struct House;
struct Targets;
struct Weapon;
struct ColliderCtx;
struct WorldCtx;
struct PlayerCtx;
struct WeaponEvents;

using std::vector;

struct World
{
    World(
        Player &player,
        Camera &cam,
        ColliderCtx &ctx,
        WorldCtx &worldCtx,
        World &world,
        Gizmos &gizmos);
        
    glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);

    int grid_width = 64;
    float grid_square_size = 64.0f;

    std::vector<Cell> cells;
    std::vector<Weapon> weapons;
    std::vector<House> structures;

    struct Compass
    {
        float o = 100.0; //z offset
        glm::vec4 color[3] = {
            {1, 0, 0, 0.8},
            {0, 1, 0, 0.8},
            {0, 0, 1, 0.8}};

        std::vector<Line> lines = {
            {{{0, 0, 0}, color[0]},
             {{100, 0, 0}, color[0]}},
            {{{0, 0, 0}, color[1]},
             {{0, 100, 0}, color[1]}},
            {{{0, 0, 0}, color[2]},
             {{0, 0, 100}, color[2]}}};
    } compass;


    void InitWorld(ColliderCtx &ctx, WorldCtx &worldCtx, Gizmos &gizmos);
    void InitCompas(std::vector<Point> &line_pts);
    void InitCube(ColliderCtx &ctx, WorldCtx &worldCtx, std::vector<Shape> &cubes);
};

void InitGrid(std::vector<Point> &line_pts, glm::vec2 origin, int amount, float cellSize);

struct Cell
{
    int id;
    glm::vec3 pos;
};

struct House
{
    entt::entity id;
    glm::vec2 pos;
    int cell;
};

Projectile CreateProjectile(vector<Projectile> &list, Projectile &pro, entt::registry &colliders);
Beam CreateBeam(vector<Beam> &list, Beam &beam, entt::registry &colliders);
Blast CreateBlast(vector<Blast> &list, Blast &blast, entt::registry &colliders);

struct WorldEnv
{
    vector<Blast> explosions;
};

struct WorldEvents
{
    vector<Hit> hits;
};

struct WorldCtx
{
    std::vector<Targets> targets;

    struct Projectiles
    {
        std::vector<Projectile> list;
    } projectiles;
    void UpdateProjectiles(float time_elapsed, Gizmos &gizmos, WorldEvents &worldQue, WorldEnv &wrldFXque, entt::registry &colliders, Weapon &weapon);
    void EraseProjectiles();

    struct Blasts
    {
        std::vector<Blast> list;
    } blasts;
    void UpdateBlasts(float time_elapsed, std::vector<Shape> &circles);
    void EraseBlasts();

    struct Beams
    {
        std::vector<Beam> list;
    } beams;
    void UpdateBeams(float time_elapsed, std::vector<Line> &lines, std::vector<Beam> &Beams, WorldEvents &worldque,WorldEnv &worldFXque, entt::registry &colliders);
    void EraseBeams();
};


struct Targets
{
    entt::entity collider;
    vec3 pos;
    float size;
    float max_hp;
    float hp;
    bool show_info = false;
    bool showing_info = false;
    bool show_hitbox = false;
    bool showing_hitbox = false;
};

struct Selector
{
    size_t ID = 0;
    bool dragging = false;
    vec3 offset{0.0};
};

void EraseEntt(entt::registry &reg);
void UpdateWorldTargets(entt::registry &colliders, PlayerCtx &ctx, Input &input, WorldCtx &world);
void UpdateWorldEdit(entt::registry &colliders, PlayerCtx &ctx, Input &input, WorldCtx &worldCtx, Gizmos &gizmos);
void WorldCreateQueue(vector<WeaponEvents> &weapQue, WorldCtx &ctx, entt::registry &colliders);
void WorldFXQueue(WorldCtx &ctx, WorldEnv &fxque, entt::registry &colliders);
void WorldHitQueue(WorldEvents &que, WorldCtx &ctx);
void WorldClearQueue(vector<WeaponEvents> &wepque,  WorldEnv &worldEnv);

void LoadNoiseForGrass();
void SpawnGrass(Gizmos &gizmos);

#endif