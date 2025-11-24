#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

// #include "app.h"
#include "helper/jsonHelper.h"
#include "world.h"
#include "loadmap.h"
#include "weapon.h"
#include "camera.h"
#include "player.h"
#include "playerCtx.h"
#include "gizmos.h"
#include "collisions.h"
#include "weaponEventQueue.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void loadMap(Gizmos &gizmos, entt::registry &colliders, World &world)
{
    std::cout << "got here" << std::endl;
    json data = parseFile();
    // Example of accessing data from the JSON
    if (data.contains("map"))
    {
        json map = data["map"];
        std::cout << "found map" << std::endl;
        if (map.contains("name"))
        {
            std::string name = map["name"];
            std::cout << "name: " << name << std::endl;
            for (const auto &object : map["objects"])
            {
                std::string type = object["type"];
                std::cout << "type: " << type << std::endl;
                std::string id = object["id"];
                std::cout << "id: " << id << std::endl;
                // std::string pos_string = object["position"];
                // std::string radius_string = object["radius"];

                glm::vec3 pos;
                if (object.contains("position") && object["position"].is_array())
                {
                    pos.x = object["position"][0].get<float>();
                    pos.y = object["position"][1].get<float>();
                    pos.z = object["position"][2].get<float>();
                }

                float size;
                float radius;
                if (object.contains("size"))
                {
                    size = object["size"].get<float>();
                }

                float hp;
                if (object.contains("hp"))
                {
                    hp = object["hp"].get<float>();
                }

                radius = size / 2;

                pos.z += radius;

                glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

                Sphere sphere{
                    pos,
                    radius,
                    color};

                glm::vec3 box_min = pos - glm::vec3(radius);
                glm::vec3 box_max = pos + glm::vec3(radius);
                vec3 yellow{1.0, 1.0, 0.0};
                glm::vec3 corners[8] = {
                    {box_min.x, box_min.y, box_min.z},
                    {box_max.x, box_min.y, box_min.z},
                    {box_max.x, box_max.y, box_min.z},
                    {box_min.x, box_max.y, box_min.z},
                    {box_min.x, box_min.y, box_max.z},
                    {box_max.x, box_min.y, box_max.z},
                    {box_max.x, box_max.y, box_max.z},
                    {box_min.x, box_max.y, box_max.z},
                };

                // 12 edges
                int edges[12][2] = {
                    {0, 1}, {1, 2}, {2, 3}, {3, 0}, // bottom
                    {4, 5},
                    {5, 6},
                    {6, 7},
                    {7, 4}, // top
                    {0, 4},
                    {1, 5},
                    {2, 6},
                    {3, 7} // verticals
                };

                for (int i = 0; i < 12; ++i)
                {
                    int a = edges[i][0];
                    int b = edges[i][1];

                    gizmos.static_line_points.push_back({corners[a], yellow});
                    gizmos.static_line_points.push_back({corners[b], yellow});
                }

                /*
                                Line lines[12] = {
                                    {corners[0], corners[1]},
                                    {corners[1], corners[2]},
                                    {corners[2], corners[3]},
                                    {corners[3], corners[0]},
                                    {corners[4], corners[5]},
                                    {corners[5], corners[6]},
                                    {corners[6], corners[7]},
                                    {corners[7], corners[4]},
                                    {corners[0], corners[4]},
                                    {corners[1], corners[5]},
                                    {corners[2], corners[6]},
                                    {corners[3], corners[7]},
                                }; */

                entt::entity hitbox = colliders.create();

                Targets target{
                    hitbox,
                    pos,
                    size,
                    hp};

                colliders.emplace<BoxColliderAxis>(hitbox,
                                                   box_min,
                                                   box_max);

                world.targets.push_back(target);

                gizmos.spheres.push_back(sphere);
            }
        }
    }
}

void World::InitializeWorld(
    Player &player,
    Camera &cam,
    ColliderCtx &ctx,
    World &world,
    Gizmos &gizmos,
    GLuint &shaderID,
    GLuint &vbo_points,
    GLuint &vbo_lines,
    entt::registry &registry)
{

    loadMap(gizmos, ctx.collidables, world);
    Ground ground{{-256.0, -5.0, -256.0}, 8};
    gizmos.ground.push_back(ground);

    cam.SetCam(shaderID);

    // player
    player.pos.x = 0.0f;
    player.pos.y = 0.0f;
    Weapon weapon;

    weapon.id = registry.create();
    registry.emplace<Weapon>(weapon.id);
    registry.emplace<LaserComponent>(weapon.id);
    registry.emplace<BlastComponent>(weapon.id);
    registry.emplace<ProjectileComponent>(weapon.id);

    // weapons.push_back(weapon);

    player.weapon = weapon.id;

    gizmos.points.push_back(player.pos_dot);
    gizmos.points.push_back(player.xhair_dot);
    gizmos.capsules.push_back(player.playerCap);

    cells.reserve(grid_width * grid_width);
    for (int y = 0; y < grid_width; ++y)
    {
        for (int x = 0; x < grid_width; ++x)
        {
            Cell cell;
            cell.id = y * grid_width + x;

            glm::vec3 start = origin;
            start.x -= (grid_width * grid_square_size) / 2.0f;
            start.y -= (grid_width * grid_square_size) / 2.0f;
            cell.pos = start + glm::vec3(x + 0.5f, y + 0.5f, 0.0f) * grid_square_size;

            cells.push_back(cell);
        }
    }
    std::cout << "cells.size(): " << cells.size() << std::endl;

    House house;
    house.cell = 1820;
    structures.push_back(house);

    InitWorld(ctx, gizmos);

    std::cout << "Initialized world." << std::endl;
}

void World::InitWorld(ColliderCtx &ctx, Gizmos &gizmos)
{
    InitGrid(gizmos.static_line_points, glm::vec2(0.0f, 0.0f), grid_width, grid_square_size);

    InitCompas(gizmos.static_line_points);

    World::InitCube(ctx, gizmos.cubes);
}

void World::InitCube(ColliderCtx &ctx, std::vector<Cube> &cubes)
{
    auto &reg = ctx.collidables;

    glm::vec3 color = {0.0f, 1.0f, 1.0f};
    float size = 100.0f;

    std::cout << "trying to find cell" << std::endl;

    for (auto &cell : cells)
        if (cell.id == 1820)
        {
            glm::vec3 center = cell.pos;
            center.z += size / 2;
            std::cout << "found cell" << center.x << center.y << center.z << std::endl;
            for (auto &house : structures)
            {
                std::cout << "found house" << std::endl;
                Cube cube = CreateCube(center, size, color, cubes);

                glm::vec3 offset_cube_start = center;
                glm::vec3 offset_cube_end = center;

                offset_cube_start.x -= size / 2;
                offset_cube_start.y -= size / 2;
                offset_cube_start.z -= size / 2;
                offset_cube_end.x += size / 2;
                offset_cube_end.y += size / 2;
                offset_cube_end.z += size / 2;

                house.id = reg.create();
                reg.emplace<BoxColliderAxis>(house.id, offset_cube_start, offset_cube_end);
            }
        }
}
void World::InitCompas(std::vector<Point> &lines)
{
    for (auto line : compass.lines)
    {
        Point a, b;
        a.pos = line.start;
        a.color = line.color;

        b.pos = line.end;
        b.color = line.color;
        lines.push_back(a);
        lines.push_back(b);
    }
}

void DrawCell(float x, float y, float cellSize)
{ /* re implement
   glBegin(GL_QUADS);
   glVertex2f(x, y);
   glVertex2f(x + cellSize, y);
   glVertex2f(x + cellSize, y + cellSize);
   glVertex2f(x, y + cellSize);
   glEnd(); */
}

void InitGrid(std::vector<Point> &lines, glm::vec2 origin, int width, float cellSize)
{

    glm::vec2 start = origin - (width * cellSize) / 2.0f;
    glLineWidth(1.0f);
    glm::vec3 color = {0.2f, 0.2f, 0.2f};

    for (int amount = 0; amount <= width; ++amount)
    {
        float x = start.x + amount * cellSize;
        float y_start = start.y;
        float y_end = start.y + width * cellSize;
        Point a, b;
        a.color = color;
        b.color = color;
        a.pos = glm::vec3(x, y_start, 0.0);
        b.pos = glm::vec3(x, y_end, 0.0);
        lines.push_back(a);
        lines.push_back(b);
    }
    // draw horizontal lines
    for (int amount = 0; amount <= width; ++amount)
    {
        float y = start.y + amount * cellSize;
        float x_start = start.x;
        float x_end = start.x + width * cellSize;
        Point a, b;
        a.color = color;
        b.color = color;
        a.pos = glm::vec3(x_start, y, 0.0);
        b.pos = glm::vec3(x_end, y, 0.0);
        lines.push_back(a);
        lines.push_back(b);
    }
}

void UpdateWorldTargets(entt::registry &colliders, PlayerCtx &ctx, World &world)
{
    for (auto &target : world.targets)
    {
        auto &hitbox = colliders.get<BoxColliderAxis>(target.collider);
        bool hit = MouseHit(ctx.xhair, hitbox.start, hitbox.end);
        if (hit && !target.showing_info)
        {
            std::cout << "hit target!" << std::endl;
        }
        if (!hit && target.showing_info)
        {
            std::cout << "left target!" << std::endl;
        }
        target.show_info = hit;
        target.showing_info = hit;
    }
}

void WorldQueue(WeaponEvents &que, WorldCtx &ctx)
{
    for (auto &p : que.projectiles)
        CreateProjectile(ctx.projectiles.list, p.pos, p.direction, p.speed, p.range);

    for (auto &l : que.lasers)
        CreateLaser(ctx.lasers.list, l.pos_start, l.pos_end, l.cooldown);

    for (auto &b : que.blasts)
        CreateBlast(ctx.blasts.list, b.pos, b.max_size, b.cooldown);

    que.projectiles.clear();
    que.lasers.clear();
    que.blasts.clear();
}

void CreateProjectile(vector<Projectile> &list, vec3 pos, vec3 direction, float speed, float range)
{

    Projectile p;

    p.direction = direction;
    p.pos = pos;
    p.speed = speed;
    p.range = range;

    list.push_back(p);
}

void CreateBlast(vector<Blast> &list, glm::vec3 pos, float size, float cooldown)
{
    Blast blast{size, cooldown, cooldown, pos}; // (set the cooldown and time left of blast)
    list.push_back(blast);
}

void CreateLaser(vector<Laser> &list, glm::vec3 start_pos, glm::vec3 end_pos, float cooldown)
{
    Laser laser{start_pos, end_pos, cooldown};
    list.push_back(laser);
}

void WorldCtx::UpdateProjectiles(float time_elapsed, std::vector<Point> &points)
{
    static glm::vec3 color = {1.0f, 0.0f, 1.0f};
    for (auto &proj : projectiles.list)
    {
        // update projectile
        proj.pos += proj.direction * proj.speed * time_elapsed;
        proj.distance += proj.speed * time_elapsed;
        // new point for render
        Point point;
        point.pos = proj.pos;
        point.color = color;

        points.push_back(point);
    }
}

void WorldCtx::UpdateBlasts(float time_elapsed, std::vector<Circle> &circles)
{
    static glm::vec3 color = {1.0f, 0.0f, 0.0f};

    for (auto &blast : blasts.list)
    {
        if (blast.time_left > 0.0f)
        {
            blast.time_left -= time_elapsed;

            blast.size = blast.max_size * (1.0f - float(blast.time_left / blast.cooldown));

            Circle circle{// color, center, size
                          blast.pos,
                          blast.size,
                          color};
            circles.push_back(circle);

            if (blast.time_left < 0.0f)
                blast.time_left = 0.0f;
        }
        else
            blast.time_left = 0.0;
    }
}

void WorldCtx::UpdateLasers(float time_elapsed, std::vector<Line> &lines)
{
    glm::vec3 color = {1.0f, 0.0f, 0.0f};

    for (auto &laser : lasers.list)
    {
        if (laser.cooldown > 0.0f)
        {
            laser.cooldown -= time_elapsed;

            Line line{

                laser.pos_start,
                laser.pos_end,
                color};

            lines.push_back(line);

            if (laser.cooldown < 0.0f)
                laser.cooldown = 0.0f;
        }
        else
            laser.cooldown = 0.0;
    }
}

void WorldCtx::EraseBlasts()
{
    auto &list = blasts.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Blast &b)
                       { return b.time_left <= 0.0f; }),
        list.end());
}

void WorldCtx::EraseLasers()
{
    auto &list = lasers.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Laser &l)
                       { return l.cooldown <= 0.0f; }),
        list.end());
}

void WorldCtx::EraseProjectiles()
{
    auto &list = projectiles.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Projectile &b)
                       { return b.distance >= b.range; }),
        list.end());
}