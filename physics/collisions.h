#define COLLISIONS_H
#ifdef COLLISIONS_H

#include <glm/glm.hpp>
#include <entt/entt.hpp>

using glm::vec3;

struct ColliderCtx
{
    entt::registry collidables;
};

struct BoxColliderAxis
{
    vec3 start;
    vec3 end;
};

struct CapCollider
{
    vec3 start;
    vec3 end;
    float radius;
};

struct SphereCollider
{
    vec3 pos;
    float radius;
};

struct CollisionResult
{
    float fraction = 1.0f; // how far along delta we can move
    vec3 normal;      // normal of surface we hit
    bool hit = false;      // whether we collided
};

struct MouseHit
{
    bool hit = false;
};
/* 
bool capsule_vs_box(const vec3 &cap_start, const vec3 &cap_end,
                    float radius, const vec3 &box_start, const vec3 &box_end);


bool point_vs_box(const vec3 pos, const vec3 &box_start, const vec3 &box_end); */

bool MouseHit(const vec3 pos, const vec3 &box_start, const vec3 &box_end);
CollisionResult TestCollisions(ColliderCtx &ctx, vec3 pos, vec3 next_pos, vec3 cap_start, vec3 cap_end, float radius);

#endif // COLLISIONS