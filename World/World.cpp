#include <algorithm>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <print>

#include "Weapons.h"
#include "World.h"
#include "worldLights.h"
#include "lights.h"
#include "camera.h"
#include "colliders.h"
#include "gizmos.h"
#include "glm/fwd.hpp"
#include "global.h"
#include "helper/jsonHelper.h"
#include "input.h"
#include "models.h"
#include "player.h"
#include "playerCtx.h"
#include "stb/stb_image.h"
#include "worldCtx.h"
#include "tools.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void loadMap(Gizmos &gizmos, entt::registry &colliders, WorldCtx &ctx)
{
    std::cout << "got here" << std::endl;
    json data = parseFile();
    // Example of accessing data from the JSON
    if (data.contains("map")) {
        json map = data["map"];
        std::cout << "found map" << std::endl;
        if (map.contains("name")) {
            std::string name = map["name"];
            std::cout << "name: " << name << std::endl;
            for (const auto &object : map["objects"]) {
                std::string type = object["type"];
                std::cout << "type: " << type << std::endl;
                std::string id = object["id"];
                std::cout << "id: " << id << std::endl;
                // std::string pos_string = object["position"];
                // std::string radius_string = object["radius"];

                glm::vec3 pos;
                if (object.contains("position") &&
                    object["position"].is_array()) {
                    pos.x = object["position"][0].get<float>();
                    pos.y = object["position"][1].get<float>();
                    pos.z = object["position"][2].get<float>();
                }

                float size;
                float radius;
                if (object.contains("size")) {
                    size = object["size"].get<float>();
                }

                float hp;
                if (object.contains("hp")) {
                    hp = object["hp"].get<float>();
                }

                radius = size / 2;

                pos.z += radius;

                glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);

                Shape sphere{pos, vec3(radius), color};

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

                for (int i = 0; i < 12; ++i) {
                    int a = edges[i][0];
                    int b = edges[i][1];

                    // gizmos.static_line_points.push_back({corners[a], yellow});
                    // gizmos.static_line_points.push_back({corners[b], yellow});
                }

                entt::entity hitbox = colliders.create();

                Targets target{hitbox, pos, size, hp, hp};

                colliders.emplace<BoxColliderAxis>(hitbox, box_min, box_max);

                ctx.targets.push_back(target);
                gizmos.spheres.push_back(sphere);
                std::cout << "made target" << std::endl;
            }
        }
    }
}

World::World(Player &player, Camera &cam, ColliderCtx &ctx, WorldCtx &worldCtx,
             World &world, Gizmos &gizmos)
{

    loadMap(gizmos, ctx.colliders, worldCtx);

    Plane ground;
    
    SetLights(gizmos, ctx.colliders);

    ground.transform = glm::mat4(1.0f);

    auto &tf = ground.transform;

    tf = glm::translate(tf, vec3(0.0, 0.0, 0.0));
    tf = glm::rotate(tf, glm::radians(0.0f), vec3(-1.0, 0.0, 0.0));
    tf = glm::scale(tf, vec3(2500.0, 2500.0, 0.0));

    // BoxColliderAxis ground_collider;
    // ground_collider.start = vec3{1500.0, 1500.0, 0.0};
    // ground_collider.end = vec3{-1500.0, -1500.0, 0.0};

    // entt::entity ground_entity = ctx.colliders.create();
    // ctx.colliders.emplace<BoxColliderAxis>(ground_entity, ground_collider);
    gizmos.planes.push_back(ground);

    SpawnGrass(gizmos);

    cam.SetCam();

    // player

    player.SetOrientation(cam);
    player.position.x = 0.0f;
    player.position.y = 0.0f;

    Model model("assets/bella.obj");

    Transform transform{1.0f};
    auto & p_tf = transform.transform;

    p_tf = glm::translate(p_tf, vec3(0.0, 0.0, 0.0));
    p_tf = glm::rotate(p_tf, glm::radians(0.0f), vec3(0.0, 0.0, 1.0));
    p_tf = glm::scale(p_tf, vec3(player.height));

    player.transforms.push_back(transform);
    player.models.push_back(model);

    Weapon weapon;

    weapon.add<GunPart>();
    weapon.add<BeamPart>();
    weapon.add<BlastPart>();
    weapon.add<TestComponent>();

    player.weapons.push_back(weapon);

    for (auto &mesh : player.models[0].meshes) {
        auto &color = mesh.material.diffuse;
        std::print("Player colors should be: {}\n", glm::to_string(color));
    }
    
    gizmos.points.push_back(player.pos_dot);
    gizmos.points.push_back(player.xhair_dot);
    gizmos.capsules.push_back(player.playerCap);
    gizmos.static_line_points.push_back(player.facing_line_start);
    gizmos.static_line_points.push_back(player.facing_line_end);

    cells.reserve(grid_width * grid_width);
    for (int y = 0; y < grid_width; ++y) {
        for (int x = 0; x < grid_width; ++x) {
            Cell cell;
            cell.id = y * grid_width + x;

            glm::vec3 start = origin;
            start.x -= (grid_width * grid_square_size) / 2.0f;
            start.y -= (grid_width * grid_square_size) / 2.0f;
            cell.pos =
                start + glm::vec3(x + 0.5f, y + 0.5f, 0.0f) * grid_square_size;

            cells.push_back(cell);
        }
    }
    // std::cout << "cells.size(): " << cells.size() << std::endl;

    House house;
    house.cell = 1820;
    structures.push_back(house);

    InitWorld(ctx, worldCtx, gizmos);

    std::cout << "Initialized world." << std::endl;
}

void World::InitWorld(ColliderCtx &ctx, WorldCtx &worldCtx, Gizmos &gizmos)
{
    InitGrid(gizmos.static_line_points, glm::vec2(0.0f, 0.0f), grid_width,
             grid_square_size);

    InitCompas(gizmos.static_line_points);

    World::InitCube(ctx, worldCtx, gizmos.cubes);
}

void World::InitCube(ColliderCtx &ctx, WorldCtx &worldCtx, vector<Shape> &cube_list)
{
    auto &reg = ctx.colliders;

    glm::vec4 color = {0, 1, 1, 1};
    Material material;
    float size = 100.0f;

    // std::cout << "trying to find cell" << std::endl;

    for (auto &cell : cells)
        if (cell.id == 1820) {
            glm::vec3 center = cell.pos;
            center.z += size / 2;
            // std::cout << "found cell" << center.x << center.y << center.z << std::endl;
            for (auto &house : structures) {
                // std::cout << "found house" << std::endl;
                CreateShape(center, vec3(size), color, cube_list);

                glm::vec3 offset_cube_start = center;
                glm::vec3 offset_cube_end = center;

                offset_cube_start -= size / 2;
                offset_cube_end += size / 2;

                house.id = reg.create();
                reg.emplace<BoxColliderAxis>(house.id, offset_cube_start,
                                             offset_cube_end);
            }
        }
}
void World::InitCompas(std::vector<Point> &line_pts)
{
    for (auto line : compass.lines) {
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
   glVertex2f(x + cellSize, y +
   cellSize); glVertex2f(x, y +
   cellSize); glEnd(); */
}

void InitGrid(std::vector<Point> &line_pts, glm::vec2 origin, int width,
              float cellSize)
{

    glm::vec2 start = origin - (width * cellSize) / 2.0f;
    glLineWidth(1.0f);
    glm::vec4 color = {0.2, 0.2, 0.2, 1};

    for (int amount = 0; amount <= width; ++amount) {
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
    for (int amount = 0; amount <= width; ++amount) {
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
    for (auto &target : worldCtx.targets) {
        auto &hitbox = colliders.get<BoxColliderAxis>(target.collider);
        bool hit = RayHitBox(input.mouse.camera_pos, input.mouse.cam_to_mouse,
                             hitbox.start, hitbox.end, input.mouse.ray_range)
                       .hit;
        if (hit)
            std::cout << "target!" << std::endl;
        if (hit && !target.showing_info) {
            std::cout << "hit target!" << std::endl;
        }
        if (!hit && target.showing_info) {
            std::cout << "left target!" << std::endl;
        }

        target.show_info = hit;
        target.showing_info = hit;

        if (target.hp <= target.max_hp) {
            target.hp += 0.5;
        }
    }
}

void UpdateWorldEdit(entt::registry &colliders, PlayerCtx &ctx, Input &input, WorldCtx &worldCtx, Gizmos &gizmos)
{
    for (auto &light : gizmos.lights) {
        Point p;
        p.pos = light.position;
        p.color = vec4{1.0};
        gizmos.points.push_back(p);
    }

    auto view = colliders.view<BoxColliderAxis, Selector>();
    for (auto enitity : view) {
        auto &box = colliders.get<BoxColliderAxis>(enitity);

        bool hit = RayHitBox(input.mouse.camera_pos, input.mouse.cam_to_mouse, box.start, box.end, input.mouse.ray_range)
                       .hit;

        if (hit) {
            vec4 color{0.0, 1.0, 1.0, 1.0};
            vec3 center = (box.start + box.end) * 0.5f;
            vec3 size = box.end - box.start;
            CreateShape(center, size, color, gizmos.wireframe_cubes);

            auto &light = colliders.get<Selector>(enitity);
            if (input.mouse.click_l) {
                vec3 light_pos;

                if (light.ID < gizmos.lights.size())
                    light_pos = gizmos.lights[light.ID].position;

                if (!light.dragging) {
                    vec3 mouse_pos = input.mouse.world_pos;
                    light.offset = light_pos - mouse_pos;
                }

                light.dragging = true;
            } else {
                light.dragging = false;
            }
            if (light.dragging) {
                if (light.ID < gizmos.lights.size()) {
                    gizmos.lights[light.ID].position = vec4(input.mouse.world_pos, 0.0) + vec4(light.offset, 0.0);
                    glm::vec3 halfSize = size * 0.5f; // preserve original size
                    box.start = input.mouse.world_pos + light.offset - halfSize;
                    box.end = input.mouse.world_pos + light.offset + halfSize;
                }
            }
        }
    }
}

void WorldCreateQueue(vector<WeaponEvents> &weapQue, WorldCtx &ctx, entt::registry &colliders)
{
    static uint8_t i;

    if (weapQue.empty())
        i = 0;

    for (auto &event : weapQue) {
        i++;
        auto entity = colliders.create();
        for (auto &projectile : event.projectiles) {
            if (projectile.effects & WorldEffects::EFFECT_EXPLOSIVE)
                std::cout << "proj is still explosive here" << std::endl;
            colliders.emplace<Projectile>(entity, projectile);
        }

        for (auto &beam : event.beams) {

            if (beam.effects & WorldEffects::EFFECT_EXPLOSIVE)
                std::cout << "beam explosive here" << std::endl;
            colliders.emplace<Beam>(entity, beam);
        }

        for (auto &blast : event.blasts) {
            colliders.emplace<Blast>(entity, blast);
            std::cout << "created " << i << "blasts" << std::endl;
        }
        std::cout << "created " << i << " entities:" << (int)entity
                  << std::endl;
    }
    weapQue.clear();
}

void WorldFXQueue(WorldCtx &ctx, WorldEnv &fxque, entt::registry &colliders)
{
    for (auto &blst : fxque.explosions) {
        std::cout << "trying to create blast" << std::endl;
        CreateBlast(ctx.blasts.list, blst, colliders);
    }
}

void WorldHitQueue(WorldEvents &que, WorldCtx &ctx)
{
    for (auto &hit : que.hits) {
        for (auto &target : ctx.targets) {
            if (hit.target == target.collider) {
                target.hp -= hit.damage;
                std::cout << "should do damage!" << hit.damage << std::endl;
            }
        }
    }
    que.hits.clear();
}

void WorldClearQueue(vector<WeaponEvents> &weapQue, WorldEnv &worldEnv)
{
    /*
    weapQue.projectiles.clear();
    weapQue.beams.clear(); */

    worldEnv.explosions.clear();
}

void printEffects(WorldEffects effect)
{
    for (const auto &[flag, name] : WorldEffectsNames) {
        if (flag != WorldEffects::NONE &&
            (static_cast<uint8_t>(effect) & static_cast<uint8_t>(flag))) {
            std::cout << name << " ";
        }
    }
    std::cout << std::endl;
}

WorldEffects e = static_cast<WorldEffects>(
    static_cast<uint8_t>(WorldEffects::EFFECT_EXPLOSIVE));

Projectile CreateProjectile(vector<Projectile> &list, Projectile &projectile,
                            entt::registry &colliders)
{

    list.push_back(projectile);

    if (projectile.effects == WorldEffects::EFFECT_EXPLOSIVE)
        std::cout << "ticked" << std::endl;

    // printEffects(e);

    // entt::entity projectile = colliders.create();
    // colliders.emplace<Projectile>(projectile, pctl);

    return projectile;
}

Blast CreateBlast(vector<Blast> &list, Blast &blast,
                  entt::registry &colliders)
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

void EffectBlast(entt::entity id, entt::registry &colliders, WorldEnv &que,
                 vec3 &pos)
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

void WorldCtx::UpdateProjectiles(float time_elapsed, Gizmos &gizmos,
                                 WorldEvents &worldQue, WorldEnv &FXque,
                                 entt::registry &colliders, Weapon &weapon)
{
    static glm::vec4 color = {1, 0, 1, 1};

    auto proview = colliders.view<Projectile>();

    for (auto &prjentt : /* projectiles.list */ proview) {
        auto &prj = colliders.get<Projectile>(prjentt);

        auto &pos = prj.pos;
        auto dir = prj.direction;
        auto &spd = prj.speed;
        vec3 lastpos = pos;
        vec3 nextpos = pos + dir * spd * time_elapsed;
        float distance = glm::distance(pos, nextpos);

        auto view = colliders.view<BoxColliderAxis>();
        for (auto entity : view) {
            auto &box = colliders.get<BoxColliderAxis>(entity);
            CollisionResult hit =
                RayHitBox(pos, dir, box.start, box.end, distance);
            if (hit.hit) {
                distance = distance * hit.fraction;
                nextpos = pos + dir * distance;
                prj.range = 0;
                Hit hit;
                hit.damage = prj.damage;
                hit.target = entity;
                worldQue.hits.push_back(hit);
                if (prj.effects & WorldEffects::EFFECT_EXPLOSIVE) {
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

void WorldCtx::UpdateBlasts(float time_elapsed,
                            std::vector<Shape> &circles)
{
    static glm::vec4 color = {1, 0, 0, 1};

    for (auto &blast : blasts.list) {
        if (blast.time_left > 0.0f) {
            blast.time_left -= time_elapsed;

            blast.size = blast.max_size *
                         (1.0f - float(blast.time_left / blast.cooldown));

            Shape circle{blast.pos, vec3(blast.size), color};
            circles.push_back(circle);

            if (blast.time_left < 0.0f)
                blast.time_left = 0.0f;
        } else
            blast.time_left = 0.0;
    }
}

void WorldCtx::UpdateBeams(float time_elapsed, std::vector<Line> &lines,
                           std::vector<Beam> &beams, WorldEvents &worldQue,
                           WorldEnv &worldFXque, entt::registry &colliders)
{
    static glm::vec4 color = {1, 0, 0, 1};

    auto beamview = colliders.view<Beam>();

    for (auto &beamentt : /* projectiles.list */ beamview) {
        auto &beam = colliders.get<Beam>(beamentt);
        /*  for (auto &beam : beams)
         { */
        if (!beam.hit) {
            auto view = colliders.view<BoxColliderAxis>();
            for (auto entity : view) {
                auto &box = colliders.get<BoxColliderAxis>(entity);

                auto direction = glm::normalize(vec3(beam.end - beam.start));
                float distance = glm::distance(beam.start, beam.end);
                CollisionResult hit =
                    RayHitBox(beam.start, direction, box.start, box.end, distance);
                if (hit.hit) {
                    // std::cout << "hit!" << std::endl;
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
        if (beam.cooldown > 0.0f) {
            beam.cooldown -= time_elapsed;

            Point a{start, color};
            Point b{end, color};
            Line line{a, b};

            lines.push_back(line);

            if (beam.cooldown < 0.0f)
                beam.remove = true;
        } else
            beam.cooldown = 0.0;
    }
}

void WorldCtx::EraseBlasts()
{
    auto &list = blasts.list;
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](const Blast &b) { return b.time_left <= 0.0f; }),
        list.end());
}

void EraseEntt(entt::registry &reg)
{
    auto projectiles = reg.view<Projectile>();
    for (auto entity : projectiles) {
        auto &projectile = reg.get<Projectile>(entity);
        if (projectile.remove)
            reg.destroy(entity);
    }

    auto beams = reg.view<Beam>();
    for (auto entity : beams) {
        auto &beam = reg.get<Beam>(entity);
        if (beam.remove)
            reg.destroy(entity);
    }
}

void WorldCtx::EraseBeams()
{
    auto &list = beams.list;
    list.erase(std::remove_if(list.begin(), list.end(),
                              [](const Beam &b) { return b.remove; }),
               list.end());
}

void WorldCtx::EraseProjectiles()
{
    auto &list = projectiles.list;
    list.erase(std::remove_if(list.begin(), list.end(),
                              [](const Projectile &b) { return b.remove; }),
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
    /*
        if (data) {
            std::cout << "Loaded" << std::endl;
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
     */
    float instances = 1;  // one grass per scan
    float maxUp = 3;      // max grass deviation from ground
    float maxHeight = 32; // max grass height

    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    float planeWidth = 2000;  // world plane size
    float planeHeight = 2000; // world plane size

    float texture_repeat =
        1; // use less or more of the texture, ie less than 1 or repeat if
           // greater than 1 (can just skip for now for 1)
    float scan_res =
        planeWidth * 0.33; // reasonable resolution of the scan in samples
    // only supporting squares for now

    size_t blade_count = 0;
    const size_t MAX_GRASS_BLADES = 400000; // just putting a cap on it

    // std::cout << "loading grass" << std::endl;

    for (int y = 0; y < scan_res; ++y) {
        for (int x = 0; x < scan_res; ++x) {

            float u = x / scan_res;
            float v = y / scan_res;
            float world_x = u * planeWidth - planeWidth * 0.5f + center.x;
            float world_y = v * planeHeight - planeHeight * 0.5f + center.y;

            float intensity = static_cast<float>(data[y * width + x]) / 255.0f;

            int numInstances = int(intensity * instances + 0.5f);

            for (int i = 0; i < numInstances && blade_count < MAX_GRASS_BLADES;
                 ++i) {
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
                // glm::vec4 color{0.5, 0.1, 0.1, 1};
                GrassBlade grass{pos, size, color};

                gizmos.grass.push_back(grass);
                blade_count++;
            }
        }
    }
    std::cout << "grass: " << gizmos.grass.size() << std::endl;
    stbi_image_free(data);
}