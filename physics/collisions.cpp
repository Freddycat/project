#include <collisions.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

entt::registry collidables;

float segment_to_segment_dist_sq(const glm::vec3 &p1, const glm::vec3 &q1,
                                 const glm::vec3 &p2, const glm::vec3 &q2)
{
    glm::vec3 d1 = q1 - p1;
    glm::vec3 d2 = q2 - p2;
    glm::vec3 r = p1 - p2;
    float a = glm::dot(d1, d1);
    float e = glm::dot(d2, d2);
    float f = glm::dot(d2, r);

    float s, t;
    const float EPS = 1e-6f;

    if (a <= EPS && e <= EPS)
    {
        s = t = 0.0f;
        return glm::dot(r, r);
    }
    if (a <= EPS)
    {
        s = 0.0f;
        t = glm::clamp(f / e, 0.0f, 1.0f);
    }
    else
    {
        float c = glm::dot(d1, r);
        if (e <= EPS)
        {
            t = 0.0f;
            s = glm::clamp(-c / a, 0.0f, 1.0f);
        }
        else
        {
            float b = glm::dot(d1, d2);
            float denom = a * e - b * b;
            if (denom != 0.0f)
                s = glm::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            else
                s = 0.0f;
            t = (b * s + f) / e;
            if (t < 0.0f)
            {
                t = 0.0f;
                s = glm::clamp(-c / a, 0.0f, 1.0f);
            }
            else if (t > 1.0f)
            {
                t = 1.0f;
                s = glm::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }
    glm::vec3 c1 = p1 + d1 * s;
    glm::vec3 c2 = p2 + d2 * t;
    return glm::dot(c1 - c2, c1 - c2);
}

CollisionResult capsule_vs_box(const glm::vec3 start_pos, glm::vec3 end_pos, const glm::vec3 cap_start, const glm::vec3 cap_end,
                               const float radius, const glm::vec3 box_start, const glm::vec3 box_end)
{
    glm::vec3 delta = end_pos - start_pos;
    glm::vec3 move_vec = end_pos - start_pos;
    glm::vec3 swept_start = cap_start + move_vec;
    glm::vec3 swept_end = cap_end + move_vec;

    CollisionResult result;

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
    }
    return result;
}

/*
    auto closest_point = [&](const glm::vec3 &p)
    {
        return glm::clamp(p, box_start, box_end);
    };

    if (glm::dot(swept_start - closest_point(swept_start), swept_start - closest_point(swept_start)) <= radius * radius)
        return true;
    if (glm::dot(swept_end - closest_point(swept_end), swept_end - closest_point(swept_end)) <= radius * radius)
        return true;
 */
/* // Check closest distance from segment to rectangle edges
// rectangle edges as segments: min->(max.x,min.y), (max.x,min.y)->max, max->(min.x,max.y), (min.x,max.y)->min
glm::vec2 rect_points[4] = {min, {max.x, min.y}, max, {min.x, max.y}};
for (int i = 0; i < 4; ++i)
{
    glm::vec2 p0 = rect_points[i];
    glm::vec2 p1 = rect_points[(i + 1) % 4];
    float dist_sq = segment_to_segment_dist_sq(cap_start, cap_end, p0, p1);
    if (dist_sq <= radius * radius)
        return true;
} */

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
