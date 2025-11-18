#define COLLISIONS_H
#ifdef COLLISIONS_H

#include <glm/glm.hpp>
#include <entt/entt.hpp>

extern entt::registry collidables;

struct BoxColliderAxis
{
    glm::vec3 start;
    glm::vec3 end;
};

struct CapCollider
{
    glm::vec3 start;
    glm::vec3 end;
    float radius;
};

struct CollisionResult
{
    float fraction = 1.0f;       // how far along delta we can move
    glm::vec3 normal;      // normal of surface we hit
    bool hit = false;            // whether we collided
};

bool capsule_vs_box(const glm::vec3 &cap_start, const glm::vec3 &cap_end,
                    float radius, const glm::vec3 &box_start, const glm::vec3 &box_end);

CollisionResult TestCollisions(glm::vec3 pos, glm::vec3 next_pos, glm::vec3 cap_start, glm::vec3 cap_end, float radius);

#endif // COLLISIONS