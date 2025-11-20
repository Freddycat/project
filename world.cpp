#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#include "app.h"
#include "world.h"
#include "weapon.h"
#include "player.h"
#include "gizmos.h"
#include "collisions.h"

void World::InitializeWorld(
    Player &player,
    Camera &cam,
    Gizmos &gizmos,
    GLuint &shaderID,
    GLuint &vbo_points,
    GLuint &vbo_lines,
    entt::registry &registry)
{
    cam.SetCam(shaderID);

    // player
    player.pos.x = 0.0f;
    player.pos.y = 0.0f;
    Weapon weapon;

    weapon.id = registry.create();
    registry.emplace<Weapon>(weapon.id);
    registry.emplace<LaserComponent>(weapon.id);
    registry.emplace<BlastComponent>(weapon.id);

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

    InitWorld(gizmos);

    std::cout << "Initialized world." << std::endl;
}

void World::InitWorld(Gizmos &gizmos)
{
    InitGrid(gizmos.static_line_points, glm::vec2(0.0f, 0.0f), grid_width, grid_square_size);

    InitCompas(gizmos.static_line_points);

    World::InitCube(gizmos.cubes);
}

void World::InitCube(std::vector<Cube> &cubes)
{
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

                house.id = collidables.create();
                collidables.emplace<BoxColliderAxis>(house.id, offset_cube_start, offset_cube_end);
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

void CreateBullet(glm::vec3 target, glm::vec3 start, std::vector<Bullet> &bullets)
{
    Bullet bullet;

    glm::vec3 distance = target - start;

    bullet.direction = glm::normalize(distance);

    bullet.pos = start;
    
    bullets.push_back(bullet);
}

void UpdateBullets(double time_elapsed, std::vector<Bullet> &bullets, std::vector<Point> &points)
{
    static glm::vec3 color = {1.0f, 0.0f, 1.0f};
    for (auto &bullet : bullets)
    {
        Point point;

        bullet.pos += bullet.direction * bullet.speed * time_elapsed;

        point.pos = bullet.pos;
        point.color = color;
    }
}

void CreateLaser(glm::vec3 start_pos, glm::vec3 end_pos, double cooldown, std::vector<Laser> &lasers)
{
    Laser laser{start_pos, end_pos, cooldown};
    lasers.push_back(laser);
}

void CreateBlast(float size, float cooldown, glm::vec3 pos, std::vector<Blast> &blasts)
{
    // std::cout << "Creating blast" << std::endl;
    Blast blast{size, cooldown, cooldown, pos}; // (set the cooldown and time left of blast)
    blasts.push_back(blast);
    // std::cout << "Created blast" << std::endl;
}

void UpdateBlasts(double time_elapsed, std::vector<Blast> &blasts, std::vector<Circle> &circles)
{
    static glm::vec3 color = {1.0f, 0.0f, 0.0f};

    for (auto &blast : blasts)
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

void UpdateLasers(double time_elapsed, std::vector<Laser> &lasers, std::vector<Line> &lines)
{
    glm::vec3 color = {1.0f, 0.0f, 0.0f};

    for (auto &laser : lasers)
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

void World::EraseBlasts()
{
    blasts.erase(
        std::remove_if(blasts.begin(), blasts.end(),
                       [](const Blast &b)
                       { return b.time_left <= 0.0f; }),
        blasts.end());
}

void World::EraseLasers()
{
    lasers.erase(
        std::remove_if(lasers.begin(), lasers.end(),
                       [](const Laser &l)
                       { return l.cooldown <= 0.0f; }),
        lasers.end());
}
