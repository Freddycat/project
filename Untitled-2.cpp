
void Input::GetMouseWorldPos(Camera &camera, float offset)
{
    float ndc_x = (2.0f * mouse.screen_pos.x) / g.window_width - 1.0f;
    float ndc_y = 1.0f - (2.0f * mouse.screen_pos.y) / g.window_height;

    glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);    // these just clip screen space
    glm::vec4 ray_far_clip(ndc_x, ndc_y, 1.0f, 1.0f); // these just clip screen space
    glm::vec4 ray_near_world = camera.inverse_view * ray_clip;
    glm::vec4 ray_far_world = camera.inverse_view * ray_far_clip;

    ray_near_world /= ray_near_world.w;
    ray_far_world /= ray_far_world.w;

    vec3 origin = glm::vec3(ray_near_world);

    glm::vec3 near_point = glm::unProject(
        glm::vec3(mouse.screen_pos, 0.0f),
        camera.view,
        camera.projection,
        glm::vec4(0, 0, g.window_width, g.window_height));

    glm::vec3 far_point = glm::unProject(
        glm::vec3(mouse.screen_pos, 1.0f),
        camera.view,
        camera.projection,
        glm::vec4(0, 0, g.window_width, g.window_height));

    vec3 direction = glm::normalize(far_point - near_point);

    // vec3 direction = glm::normalize(glm::vec3(ray_far_world - ray_near_world));

    vec3 ground_start{10000.0, 10000.0, 0.0};
    vec3 ground_end{-10000.0, -10000.0, -2.0};

    float range = 8000.0;

    CollisionResult ground_hit = RayHit(origin, direction, ground_start, ground_end, range);

    // if (ground_hit.hit)
    // std::cout << " hit! " << std::endl;

    float distance = range * ground_hit.fraction;

    glm::vec3 world_pos = near_point + distance * direction;
    glm::vec3 head_pos = glm::vec3{world_pos.x, world_pos.y, world_pos.z + offset};
    glm::vec3 up{0, 0, 1};
    float t = (offset - near_point.z) / direction.z;
    glm::vec3 xhair_pos = near_point + t * direction;

    // float distance = -origin.z / direction.z;

    std::cout << " distance " << distance << std::endl;

    float distance_offset = (offset - origin.z) / direction.z;
    float camera_offset = (camera.position.z - origin.z) / direction.z;
    glm::vec3 offset_pos = origin + distance_offset * direction;

    mouse.world_pos = world_pos;
    mouse.xhair_pos = xhair_pos;
    mouse.camera_pos = origin;
    mouse.cam_to_mouse = direction;
}