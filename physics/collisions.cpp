#include <collisions.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

CollisionResult capsule_vs_box(const glm::vec3 start_pos, glm::vec3 end_pos, const glm::vec3 cap_start, const glm::vec3 cap_end,
                               const float radius, const glm::vec3 box_start, const glm::vec3 box_end)
{
    glm::vec3 delta = end_pos - start_pos;
    glm::vec3 direction = glm::normalize(delta);

    CollisionResult result;

    glm::vec3 ex_start = box_start - glm::vec3(radius);
    glm::vec3 ex_end = box_end + glm::vec3(radius);

    float tmin = 0.0f;
    float tmax = 1.0f;

    for (int i = 0; i < 3; i++)
    {
        if (std::abs(delta[i]) < 1e-8f)
        {
            if (cap_start[i] < ex_start[i] || cap_start[i] > ex_end[i])
                return result; // no hit
        }
        else
        {

            float invD = 1.0f / delta[i];

            float t1 = (ex_start[i] - start_pos[i]) * invD;
            float t2 = (ex_end[i] - start_pos[i]) * invD;

            if (t1 > t2)
                std::swap(t1, t2);

            tmin = glm::max(tmin, t1);
            tmax = glm::min(tmax, t2);

            if (tmin > tmax)
                return result; // miss
        }
    }
    // Collision occurs at tmin
    if (tmin < 0.0f || tmin > 1.0f)
        return result;

    std::cout << "hit a wall bro" << std::endl;

    result.hit = true;
    result.fraction = tmin;

    // Compute normal: find dominant axis
    glm::vec3 hitPoint = start_pos + delta * tmin;

    const float eps = 1e-4f;
    if (std::abs(hitPoint.x - ex_start.x) < eps)
        result.normal = glm::vec3(-1, 0, 0);
    else if (std::abs(hitPoint.x - ex_end.x) < eps)
        result.normal = glm::vec3(1, 0, 0);
    else if (std::abs(hitPoint.y - ex_start.y) < eps)
        result.normal = glm::vec3(0, -1, 0);
    else if (std::abs(hitPoint.y - ex_end.y) < eps)
        result.normal = glm::vec3(0, 1, 0);
    else if (std::abs(hitPoint.z - ex_start.z) < eps)
        result.normal = glm::vec3(0, 0, -1);
    else if (std::abs(hitPoint.z - ex_end.z) < eps)
        result.normal = glm::vec3(0, 0, 1);

    return result;
}

CollisionResult RayHit(const vec3 &origin, const vec3 &direction,
                       const vec3 &boxmin, const vec3 &boxmax,
                        const float range)
{

    float fraction = 1;
    float near = 0;
    float far = range;
    CollisionResult result;

    for (int i = 0; i < 3; ++i)
    {
        if (direction[i] == 0.0f)
        {
            if (origin[i] < boxmin[i] || origin[i] > boxmax[i])
            {
                return result;
            }
        }
        else
        {
            float t1 = (boxmin[i] - origin[i]) / direction[i];
            float t2 = (boxmax[i] - origin[i]) / direction[i];
            if (t1 > t2)
                std::swap(t1, t2);

            near = std::max(near, t1);
            far = std::min(far, t2);

            if (near > far)
                return result; // no intersection
            if (far < 0)
                return result; // box is behind the ray
        }
    }
    result.hit = true;
    result.fraction = glm::clamp(near / range, 0.0f, 1.0f);
    return result;
}

bool PointHit(const vec3 pos, const vec3 &box_start, const vec3 &box_end)
{
    bool hit =
        pos.x >= box_start.x && pos.x <= box_end.x &&
        pos.y >= box_start.y && pos.y <= box_end.y &&
        pos.z >= box_start.z && pos.z <= box_end.z;

    if (hit)
        std::cout << "mouse hit" << std::endl;

    return hit;
}

CollisionResult TestCollisions(ColliderCtx &ctx, glm::vec3 pos, glm::vec3 next_pos, glm::vec3 cap_start, glm::vec3 cap_end, float radius)
{
    auto &reg = ctx.collidables;
    CollisionResult result;

    for (auto &view : reg.view<BoxColliderAxis>())
    {
        auto &box = reg.get<BoxColliderAxis>(view);
        result = capsule_vs_box(pos, next_pos, cap_start, cap_end, radius, box.start, box.end);
        return result;
    }
    return result;
}
