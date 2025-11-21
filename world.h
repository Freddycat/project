#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <gizmos.h>
#include <entt/entt.hpp>

struct App;
struct Blast;
struct Bullet;
struct Laser;
struct Player;
struct Camera;
struct Cell;
struct House;
struct Weapon;

struct World
{
    glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);

    int grid_width = 64;
    float grid_square_size = 64.0f;

    std::vector<Blast> blasts;
    std::vector<Bullet> bullets;
    std::vector<Laser> lasers;
    std::vector<Cell> cells;
    std::vector<Weapon> weapons;
    std::vector<House> structures;

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
        Gizmos &gizmos,
        GLuint &shaderID,
        GLuint &vbo_point,
        GLuint &vbo_line,
        entt::registry &registry);

    void InitWorld(Gizmos &gizmos);
    void InitCompas(std::vector<Point> &lines);
    void InitCube(std::vector<Cube> &cubes);
    void EraseBlasts();
    void EraseBullets();
    void EraseLasers();
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

struct Blast
{
    float max_size;
    float cooldown;
    float time_left;
    glm::vec3 pos;
    float size;
    size_t vertex_start;
    size_t vertex_amount;
};

struct Bullet
{
    glm::vec3 direction;
    glm::vec3 pos;
    float speed;
    float range;
    float distance = 0.0f;
};

struct Laser
{
    glm::vec3 pos_start;
    glm::vec3 pos_end;
    float cooldown;
};

void CreateBullet(glm::vec3 target, glm::vec3 start, float speed, float range, std::vector<Bullet> &bullets);
void UpdateBullets(float time_elapsed, std::vector<Bullet> &bullets, std::vector<Point> &points);

void CreateBlast(float size, float cooldown, glm::vec3 pos, std::vector<Blast> &blasts);
void UpdateBlasts(float time_elapsed, std::vector<Blast> &blasts, std::vector<Circle> &circles);

void CreateLaser(glm::vec3 start_pos, glm::vec3 end_pos, float cooldown, std::vector<Laser> &lasers);
void UpdateLasers(float time_elapsed, std::vector<Laser> &lasers, std::vector<Line> &lines);


#endif