struct Object
{
    std::string type;
    std::string id;
    glm::vec3 pos;
    float size;
    float hp;
};

struct MapData
{
    std::string name;
    std::vector<Object> objects;
};

void loadMap(Gizmos &gizmos, entt::registry &colliders, WorldCtx &ctx);

void loadMap(Gizmos &gizmos, entt::registry &colliders, WorldCtx &ctx)
{
    json data = parseFile();

    if (data.contains("map"))
    {
        MapData map;
        json mapJson = data["map"];
        std::cout << "found map: " << std::endl;

        if (mapJson.contains("name"))
        {
            std::string name = mapJson["name"];
            std::cout << "name: " << name << std::endl;
            for (const auto &object : mapJson["objects"])
            {
                Object mapObject;

                std::string type = object["type"];
                std::cout << "type: " << type << std::endl;
                std::string id = object["id"];
                std::cout << "id: " << id << std::endl;

                glm::vec3 pos;
                if (object.contains("position") && object["position"].is_array())
                {
                    pos.x = object["position"][0].get<float>();
                    pos.y = object["position"][1].get<float>();
                    pos.z = object["position"][2].get<float>();
                    mapObject.pos = pos;
                }

                float size;
                float radius;
                if (object.contains("size"))
                {
                    size = object["size"].get<float>();
                    mapObject.size = size;
                }

                float hp;
                if (object.contains("hp"))
                {
                    hp = object["hp"].get<float>();
                    mapObject.hp = hp;
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
