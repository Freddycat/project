#include <print>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "worldLights.h"
#include "lights.h"
#include "colliders.h"
#include "tools.h"


void SetLights(Gizmos &gizmos, entt::registry &registry)
{
    Light a;
    a.position = vec4{100.0, 100.0, 100.0, 0.0};
    Light b;
    b.position = vec4{0.0, 100.0, 100.0, 0.0};
    Light c;
    c.position = vec4{0.0, 0.0, 100.0, 0.0};
    Light d;
    d.position = vec4{100.0, 0.0, 100.0, 0.0};

    Light a1;
    a1.position = vec4{-168.0, -255.0, 100.0, 0.0};
    Light a2;
    a2.position = vec4{-220.0, -150.0, 100.0, 0.0};

    gizmos.lights.push_back(a);
    gizmos.lights.push_back(b);
    gizmos.lights.push_back(c);
    gizmos.lights.push_back(d);
    gizmos.lights.push_back(a1);
    gizmos.lights.push_back(a2);

    Sun sun(
        vec3(0.1, 0.1, 1),
        vec3(0.033, 0.033, 0.033),
        vec3(3.0, 3.0, 3.0),
        vec3(0.8, 0.8, 0.8));
    gizmos.sun = sun;


    size_t light_ct = -1;

    for (auto &light : gizmos.lights) {
        light_ct++;
        vec3 pos = light.position;
        auto lightEntt = registry.create();
        BoxColliderAxis box{pos - vec3(10.0), pos + vec3(10.0)};
        Selector ID{light_ct};
        registry.emplace<BoxColliderAxis>(lightEntt, box);
        registry.emplace<Selector>(lightEntt, ID);
        std::print("Light position: {}\n", glm::to_string(pos));
    }

}