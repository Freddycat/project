#include <collisions.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

entt::registry collidables;

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

    /*
        for (float sweep = 0; sweep <= 1.0f; sweep += 0.01f)
        {
            glm::vec3 p0 = cap_start + delta * sweep;
            glm::vec3 p1 = cap_end + delta * sweep;
            glm::vec3 closest = glm::clamp(p0, box_start, box_end);
            glm::vec3 diff = p0 - closest;
            float dist2 = glm::dot(diff, diff);

            if (glm::dot(p0 - closest, p0 - closest) <= radius * radius)
            {
                result.fraction = sweep;

                if (glm::length2(diff) > 1e-6f)
                    result.normal = glm::normalize(diff);
                else
                    result.normal = glm::vec3(0.0f, 0.0f, 1.0f); // fallback

                result.hit = true;
                std::cout << "true" << std::endl;
                break;
            }
        } */
    return result;
}

CollisionResult TestCollisions(glm::vec3 pos, glm::vec3 next_pos, glm::vec3 cap_start, glm::vec3 cap_end, float radius)
{
    CollisionResult result;
    for (auto &view : collidables.view<BoxColliderAxis>())
    {
        auto &box = collidables.get<BoxColliderAxis>(view);
        result = capsule_vs_box(pos, next_pos, cap_start, cap_end, radius, box.start, box.end);
        return result;
    }
    return result;
}
