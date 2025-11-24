#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <gizmos.h>
#include <entt/entt.hpp>

#include "worldCtx.h"

struct App;
struct Blast;
struct Projectile;
struct Laser;
struct Player;
struct Camera;
struct Cell;
struct House;
struct Targets;
struct Weapon;
struct ColliderCtx;
struct PlayerCtx;
struct WeaponEvents;
struct Ground;

using std::vector;

struct World
{
    glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);

    int grid_width = 64;
    float grid_square_size = 64.0f;

    std::vector<Cell> cells;
    std::vector<Weapon> weapons;
    std::vector<House> structures;
    std::vector<Targets> targets;

    struct Compass
    {
        glm::vec3 color[3] = {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}};

        std::vector<Line> lines = {
            // start, end, color
            {{0, 0, 0}, {100, 0, 0}, color[0]}, // X axis
            {{0, 0, 0}, {0, 100, 0}, color[1]}, // Y axis
            {{0, 0, 0}, {0, 0, 100}, color[2]}  // Z axis
        };
    } compass;

    void InitializeWorld(
        Player &player,
        Camera &cam,
        ColliderCtx &ctx,
        World &world,
        Gizmos &gizmos,
        GLuint &shaderID,
        GLuint &vbo_point,
        GLuint &vbo_line,
        entt::registry &registry);

    void InitWorld(ColliderCtx &ctx, Gizmos &gizmos);
    void InitCompas(std::vector<Point> &lines);
    void InitCube(ColliderCtx &ctx, std::vector<Cube> &cubes);
};

void InitGrid(std::vector<Point> &lines, glm::vec2 origin, int amount, float cellSize);

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

void CreateProjectile(vector<Projectile> &list, vec3 pos, vec3 direction, float speed, float range);
void CreateBlast(vector<Blast> &list, glm::vec3 pos, float size, float cooldown);
void CreateLaser(vector<Laser> &list, glm::vec3 start_pos, glm::vec3 end_pos, float cooldown);

struct WorldCtx
{
    struct Projectiles
    {
        std::vector<Projectile> list;
    } projectiles;
    void UpdateProjectiles(float time_elapsed, std::vector<Point> &points);
    void EraseProjectiles();

    struct Blasts
    {
        std::vector<Blast> list;
    } blasts;
    void UpdateBlasts(float time_elapsed, std::vector<Circle> &circles);
    void EraseBlasts();

    struct Lasers
    {
        std::vector<Laser> list;
    } lasers;
    void UpdateLasers(float time_elapsed, std::vector<Line> &lines);
    void EraseLasers();
};

struct Targets
{
    entt::entity collider;
    vec3 pos;
    float size;
    float hp;
    bool show_info = false;
    bool showing_info = false;
};

void UpdateWorldTargets(entt::registry &colliders, PlayerCtx &ctx, World &world);
void WorldQueue(WeaponEvents &que, WorldCtx &ctx);

#endif