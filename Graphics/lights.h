#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

using glm::vec3, glm::vec4;

struct Light {
    vec4 position = vec4{0.0};
    vec4 color = vec4{1.0};

    vec4 attenuation = vec4{
        1.0,
        0.01,
        0.012,
        0.0};

    vec4 ambient = vec4{1.0};
    vec4 diffuse = vec4{10.0};
    vec4 specular = vec4{10.0};
};

struct Sun {
    vec3 direction = vec3{0.08, 0.08, 1.0};
    vec3 ambient = vec4{0.02};
    vec3 diffuse = vec4{2.0};
    vec3 specular = vec4{1.0};
    Sun() = default;
    Sun(vec3 dir, vec3 amb, vec3 dif, vec3 spc)
        : direction(dir), ambient(amb), diffuse(dif), specular(spc) {};
};

#endif