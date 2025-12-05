#include <glad/glad.h>
#include <math.h>
#include <algorithm>
#include <iostream>

// #include "app.h"
#include "helper/jsonHelper.h"
#include "World.h"
#include "Weapons.h"
#include "camera.h"
#include "player.h"
#include "input.h"
#include "playerCtx.h"
#include "worldCtx.h"
#include "gizmos.h"
#include "collisions.h"
#include "stb/stb_image.h"
#include "global.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void loadMap(Gizmos &gizmos, entt::registry &colliders, WorldCtx &ctx)
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

                glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);

                ShapeSym sphere{
                    pos,
                    radius,
                    color};

                glm::vec3 box_min = pos - glm::vec3(radius);
                glm::vec3 box_max = pos + glm::vec3(radius);
                vec4 yellow{1, 1, 0, 1};
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

                    gizmos.static_lines.push_back({corners[a], yellow});
                    gizmos.static_lines.push_back({corners[b], yellow});
                }

                entt::entity hitbox = colliders.create();

                Targets target{
                    hitbox,
                    pos,
                    size,
                    hp,
                    hp};

                colliders.emplace<BoxColliderAxis>(hitbox,
                                                   box_min,
                                                   box_max);

                ctx.targets.push_back(target);
                gizmos.spheres.push_back(sphere);
                std::cout << "made target" << std::endl;
            }
        }
    }
}

void World::InitializeWorld(
    Player &player,
    Camera &cam,
    ColliderCtx &ctx,
    WorldCtx &worldCtx,
    World &world,
    Gizmos &gizmos,
    GLuint &vbo_points,
    GLuint &vbo_lines)
{

    loadMap(gizmos, ctx.collidables, worldCtx);

    Plane ground;

    ground.transform = glm::mat4(1.0f);

    auto &tf = ground.transform;

    tf = glm::translate(tf, vec3(0.0, 0.0, 0.0));
    tf = glm::rotate(tf, glm::radians(0.0f), vec3(-1.0, 0.0, 0.0));
    tf = glm::scale(tf, vec3(2500.0, 2500.0, 0.0));

    BoxColliderAxis ground_collider;
    ground_collider.start = vec3{1500.0, 1500.0, 0.0};
    ground_collider.end = vec3{-1500.0, -1500.0, 0.0};

    entt::entity ground_entity = ctx.collidables.create();
    ctx.collidables.emplace<BoxColliderAxis>(ground_entity, ground_collider);
    gizmos.plane.push_back(ground);

    SpawnGrass(gizmos);
    // GrassBlade test{glm::vec3(0, 0, 0), glm::vec3(30, 30, 30), glm::vec4(0, 1, 0, 1)};
    // gizmos.grass.push_back(test);

    cam.SetCam();

    // player
    player.SetOrientation(cam);
    player.position.x = 0.0f;
    player.position.y = 0.0f;
    // Weapon weapon;

    // auto weapon_entt = registry.create();

    // auto &weaponHandle = registry.emplace<WeaponHandle>(weapon_entt);

    // weaponHandle.weapon = std::make_unique<Weapon>();

    // auto &weapon = *weaponHandle.weapon;

    Weapon weapon;

    weapon.add<GunPart>();
    weapon.add<BeamPart>();
    weapon.add<BlastPart>();
    weapon.add<TestComponent>();

    if (auto *blast = weapon.get<BlastPart>())
    { // Assuming BlastPart has a member uint8_t affects;
        // and assuming GunPart maps to 1 and BeamPart maps to 2 in the corrected enum.

        blast->affects |= PART_MASK<GunPart>::value;
        // blast->affects is now 1 (0x1)
        std::cout << "blast->affects after GUN: Dec=" << blast->affects
                  << ", Hex=0x" << std::hex << blast->affects << std::dec << std::endl;

        /*  blast->affects |= PART_MASK<BeamPart>::value;
         // blast->affects is now 1 | 2 = 3 (0x3)
         std::cout << "blast->affects after BEAM: Dec=" << blast->affects
                   << ", Hex=0x" << std::hex << blast->affects << std::dec << std::endl; */
    }

    player.weapons.push_back(weapon);

    // weapon.id = weapon_entt;
    // player.weapon = weapon_entt;

    // entt::entity prtl_part = registry.create();
    // entt::entity beam_part = registry.create();
    // registry.emplace<BeamPart>(weapon.id);
    // registry.emplace<GunPart>(weapon.id);
    // registry.emplace<BlastPart>(prtl_part);
    // registry.emplace<BlastPart>(beam_part);
    // auto &prtl_component = registry.get<GunPart>(weapon.id);
    // auto &beam_component = registry.get<BeamPart>(weapon.id);
    // beam_component.id = beam_part;

    gizmos.points.push_back(player.pos_dot);
    gizmos.points.push_back(player.xhair_dot);
    gizmos.capsules.push_back(player.playerCap);
    gizmos.static_line_points.push_back(player.facing_line_start);
    gizmos.static_line_points.push_back(player.facing_line_end);

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

    InitWorld(ctx, worldCtx, gizmos);

    std::cout << "Initialized world." << std::endl;
}

void World::InitWorld(ColliderCtx &ctx, WorldCtx &worldCtx, Gizmos &gizmos)
{
    InitGrid(gizmos.static_line_points, glm::vec2(0.0f, 0.0f), grid_width, grid_square_size);

    InitCompas(gizmos.static_line_points);

    World::InitCube(ctx, worldCtx, gizmos.cubes);
}

void World::InitCube(ColliderCtx &ctx, WorldCtx &worldCtx, std::vector<ShapeSym> &cubes)
{
    auto &reg = ctx.collidables;

    glm::vec4 color = {0, 1, 1, 1};
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
                CreateShapeSym(center, size, color, cubes);

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
void World::InitCompas(std::vector<Point> &line_pts)
{
    for (auto line : compass.lines)
    {
        line.start.pos.z += compass.o;
        line.end.pos.z += compass.o;
        line_pts.push_back(line.start);
        line_pts.push_back(line.end);
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

void InitGrid(std::vector<Point> &line_pts, glm::vec2 origin, int width, float cellSize)
{

    glm::vec2 start = origin - (width * cellSize) / 2.0f;
    glLineWidth(1.0f);
    glm::vec4 color = {0.2, 0.2, 0.2, 1};

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
        line_pts.push_back(a);
        line_pts.push_back(b);
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
        line_pts.push_back(a);
        line_pts.push_back(b);
    }
}

void UpdateWorldTargets(entt::registry &colliders, PlayerCtx &ctx, Input &input, WorldCtx &worldCtx)
{
    for (auto &target : worldCtx.targets)
    {
        auto &hitbox = colliders.get<BoxColliderAxis>(target.collider);
        bool hit = RayHit(input.mouse.camera_pos, input.mouse.cam_to_mouse, hitbox.start, hitbox.end, input.mouse.ray_range).hit;
        if (hit)
            std::cout << "target!" << std::endl;
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
        if (target.hp <= target.max_hp)
        {
            target.hp += 0.5;
        }
    }
}

void WorldCreateQueue(vector<WeaponEvent> &weapQue, WorldCtx &ctx, entt::registry &colliders)
{
    static uint8_t i;

    if (weapQue.empty())
        i = 0;

    for (auto &event : weapQue)
    {
        i++;
        auto entity = colliders.create();
        for (auto &projectile : event.projectiles)
        {
            if (projectile.effects & WorldEffects::EFFECT_EXPLOSIVE)
                std::cout << "proj is still explosive here" << std::endl;
            colliders.emplace<Projectile>(entity, projectile);
        }

        for (auto &beam : event.beams)
        {

            if (beam.effects & WorldEffects::EFFECT_EXPLOSIVE)
                std::cout << "beam explosive here" << std::endl;
            colliders.emplace<Beam>(entity, beam);
        }

        for (auto &blast : event.blasts)
        {
            colliders.emplace<Blast>(entity, blast);
            std::cout << "created " << i << "blasts" << std::endl;
        }
        std::cout << "created " << i << " entities:" << (int)entity << std::endl;
    }
    weapQue.clear();
}

void WorldFXQueue(WorldCtx &ctx, WorldFX &fxque, entt::registry &colliders)
{
    for (auto &blst : fxque.explosions)
    {
        std::cout << "trying to create blast" << std::endl;
        CreateBlast(ctx.blasts.list, blst, colliders);
    }
}

void WorldHitQueue(WorldEvents &que, WorldCtx &ctx)
{
    for (auto &hit : que.hits)
    {
        for (auto &target : ctx.targets)
        {
            if (hit.target == target.collider)
            {
                target.hp -= hit.damage;
                std::cout << "should do damage!" << hit.damage << std::endl;
            }
        }
    }
    que.hits.clear();
}

void WorldClearQueue(vector<WeaponEvent> &weapQue, WorldFX &worldFX)
{
    /*
    weapQue.projectiles.clear();
    weapQue.beams.clear(); */

    worldFX.explosions.clear();
}

void printEffects(WorldEffects effect)
{
    for (const auto &[flag, name] : WorldEffectsNames)
    {
        if (flag != WorldEffects::NONE &&
            (static_cast<uint8_t>(effect) & static_cast<uint8_t>(flag)))
        {
            std::cout << name << " ";
        }
    }
    std::cout << std::endl;
}

WorldEffects e = static_cast<WorldEffects>(
    static_cast<uint8_t>(WorldEffects::EFFECT_EXPLOSIVE));

Projectile CreateProjectile(vector<Projectile> &list, Projectile &projectile, entt::registry &colliders)
{

    list.push_back(projectile);

    if (projectile.effects == WorldEffects::EFFECT_EXPLOSIVE)
        std::cout << "ticked" << std::endl;

    // printEffects(e);

    // entt::entity projectile = colliders.create();
    // colliders.emplace<Projectile>(projectile, pctl);

    return projectile;
}

Blast CreateBlast(vector<Blast> &list, Blast &blast, entt::registry &colliders)
{
    list.push_back(blast);
    std::cout << "created blast" << std::endl;
    return blast;
}

Beam CreateBeam(vector<Beam> &list, Beam &beam, entt::registry &colliders)
{
    entt::entity entt = colliders.create();
    colliders.emplace<Beam>(entt, beam);
    list.push_back(beam);
    return beam;
}

void EffectBlast(entt::entity id, entt::registry &colliders, WorldFX &que, vec3 &pos)
{
    auto &data = colliders.get<Blast>(id);

    Blast blast;
    blast.pos = pos;
    blast.cooldown = data.cooldown;
    blast.time_left = data.cooldown;
    blast.damage = data.damage;
    blast.max_size = data.max_size;

    que.explosions.push_back(blast);
}

void WorldCtx::UpdateProjectiles(float time_elapsed, Gizmos &gizmos, WorldEvents &worldQue, WorldFX &FXque, entt::registry &colliders, Weapon &weapon)
{
    static glm::vec4 color = {1, 0, 1, 1};

    auto proview = colliders.view<Projectile>();

    for (auto &prjentt : /* projectiles.list */ proview)
    {
        auto &prj = colliders.get<Projectile>(prjentt);

        auto &pos = prj.pos;
        auto dir = prj.direction;
        auto &spd = prj.speed;
        vec3 lastpos = pos;
        vec3 nextpos = pos + dir * spd * time_elapsed;
        float distance = glm::distance(pos, nextpos);

        auto view = colliders.view<BoxColliderAxis>();
        for (auto entity : view)
        {
            auto &box = colliders.get<BoxColliderAxis>(entity);
            CollisionResult hit = RayHit(pos, dir, box.start, box.end, distance);
            if (hit.hit)
            {
                distance = distance * hit.fraction;
                nextpos = pos + dir * distance;
                prj.range = 0;
                Hit hit;
                hit.damage = prj.damage;
                hit.target = entity;
                worldQue.hits.push_back(hit);
                if (prj.effects & WorldEffects::EFFECT_EXPLOSIVE)
                {
                    std::cout << "should explode" << std::endl;
                    EffectBlast(prjentt, colliders, FXque, prj.pos);
                }
                prj.remove = true;
            }
        }
        // update projectile
        if (prj.distance >= prj.range)
            prj.remove = true;

        pos = nextpos;
        prj.distance += spd * time_elapsed;
        // new point for render
        Point point;
        point.pos = pos;
        point.color = color;

        vec4 red = {1, 0, 0, 1};

        Line line;
        Point p1{pos, red};
        Point p2{lastpos, red};
        line.start = p1;
        line.end = p2;

        gizmos.points.push_back(point);
        gizmos.lines.push_back(line);
    }
}

void WorldCtx::UpdateBlasts(float time_elapsed, std::vector<ShapeSym> &circles)
{
    static glm::vec4 color = {1, 0, 0, 1};

    for (auto &blast : blasts.list)
    {
        if (blast.time_left > 0.0f)
        {
            blast.time_left -= time_elapsed;

            blast.size = blast.max_size * (1.0f - float(blast.time_left / blast.cooldown));

            ShapeSym circle{// color, center, size
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

void WorldCtx::UpdateBeams(float time_elapsed, std::vector<Line> &lines, std::vector<Beam> &beams, WorldEvents &worldQue, WorldFX &worldFXque, entt::registry &colliders)
{
    static glm::vec4 color = {1, 0, 0, 1};

    auto beamview = colliders.view<Beam>();

    for (auto &beamentt : /* projectiles.list */ beamview)
    {
        auto &beam = colliders.get<Beam>(beamentt);
        /*  for (auto &beam : beams)
         { */
        if (!beam.hit)
        {
            auto view = colliders.view<BoxColliderAxis>();
            for (auto entity : view)
            {
                auto &box = colliders.get<BoxColliderAxis>(entity);

                auto direction = glm::normalize(vec3(beam.end - beam.start));
                float distance = glm::distance(beam.start, beam.end);
                CollisionResult hit = RayHit(beam.start, direction, box.start, box.end, distance);
                if (hit.hit)
                {
                    //std::cout << "hit!" << std::endl;
                    distance = distance * hit.fraction;
                    beam.end = beam.start + direction * distance;
                    Hit hit;
                    hit.damage = beam.damage;
                    hit.target = entity;
                    hit.weapon = beam.weapon;
                    worldQue.hits.push_back(hit);
                    beam.hit = true;
                    if (beam.effects & WorldEffects::EFFECT_EXPLOSIVE)
                        EffectBlast(beamentt, colliders, worldFXque, beam.end);
                }
            }
        }
        auto start = beam.start;
        auto end = beam.end;
        if (beam.cooldown > 0.0f)
        {
            beam.cooldown -= time_elapsed;

            Point a{start, color};
            Point b{end, color};
            Line line{a, b};

            lines.push_back(line);

            if (beam.cooldown < 0.0f)
                beam.remove = true;
        }
        else
            beam.cooldown = 0.0;
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

void EraseEntt(entt::registry &reg)
{
    auto projectiles = reg.view<Projectile>();
    for (auto entity : projectiles)
    {
        auto &projectile = reg.get<Projectile>(entity);
        if (projectile.remove)
            reg.destroy(entity);
    }

    auto beams = reg.view<Beam>();
    for (auto entity : beams)
    {
        auto &beam = reg.get<Beam>(entity);
        if (beam.remove)
            reg.destroy(entity);
    }
}

void WorldCtx::EraseBeams()
{
    auto &list = beams.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Beam &b)
                       { return b.remove; }),
        list.end());
}

void WorldCtx::EraseProjectiles()
{
    auto &list = projectiles.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Projectile &b)
                       { return b.remove; }),
        list.end());
}

void SpawnGrass(Gizmos &gizmos)
{
    auto filePath = g.home / "shaders" / "perlinNoise.png";
    const std::string path_string = filePath.string();
    const char *path = path_string.c_str();

    unsigned int texture;
    int width, height, nrChannels;

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 1);

    if (data)
    {
        std::cout << "Loaded" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    float instances = 1;  // one grass per scan
    float maxUp = 3;      // max grass deviation from ground
    float maxHeight = 32; // max grass height

    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    float planeWidth = 2000;  // world plane size
    float planeHeight = 2000; // world plane size

    float texture_repeat = 1;           // use less or more of the texture, ie less than 1 or repeat if greater than 1 (can just skip for now for 1)
    float scan_res = planeWidth * 0.33; // reasonable resolution of the scan in samples
    // only supporting squares for now

    size_t blade_count = 0;
    const size_t MAX_GRASS_BLADES = 400000; // just putting a cap on it

    std::cout << "loading grass" << std::endl;

    for (int y = 0; y < scan_res; ++y)
    {
        for (int x = 0; x < scan_res; ++x)
        {

            float u = x / scan_res;
            float v = y / scan_res;
            float world_x = u * planeWidth - planeWidth * 0.5f + center.x;
            float world_y = v * planeHeight - planeHeight * 0.5f + center.y;

            float intensity = static_cast<float>(data[y * width + x]) / 255.0f;

            int numInstances = int(intensity * instances + 0.5f);

            for (int i = 0; i < numInstances && blade_count < MAX_GRASS_BLADES; ++i)
            {
                float seed = x * 73.0f + y * 97.0f + i * 13.0f;
                float randX = glm::fract(sin(seed) * 43758.5453);
                float randY = glm::fract(sin(seed + 1.0f) * 43758.5453);

                float jitterX = randX * (500.0f / width);
                float jitterY = randY * (500.0f / height);
                glm::vec3 pos;
                pos.x = world_x + jitterX;
                pos.y = world_y + jitterY;
                pos.z = intensity * maxUp;
                glm::vec3 size;
                size.x = 2;
                size.y = 2;
                size.z = intensity * maxHeight;
                glm::vec4 color{0, 0.5, 0, 1};
                GrassBlade grass{pos, size, color};

                gizmos.grass.push_back(grass);
                blade_count++;
            }
        }
    }
    std::cout << "grass: " << gizmos.grass.size() << std::endl;
    stbi_image_free(data);
}