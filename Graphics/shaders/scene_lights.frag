#version 460 core
#extension GL_ARB_shading_language_include : enable
#include "lights.glsl"

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;

    sampler2D ambientUV;
    sampler2D diffuseUV;
    sampler2D specularUV;

    bool useAmbiUV;
    bool useDiffUV;
    bool useSpecUV;
};

struct Sun {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Sun sun;
uniform Material material;
uniform vec3 viewPos;

vec3 CalcSunLight(Sun sun, vec3 normal, vec3 viewDir)
{
    vec3 direction = normalize(sun.direction);
    // diffuse shading
    float diff = max(dot(normal, direction), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    // combine results
    vec3 ambient;
    if (material.useAmbiUV)
        ambient = sun.ambient * vec3(texture(material.diffuseUV, UV));
    else
        ambient = sun.ambient.rgb * material.ambient;
    vec3 diffuse;
    if (material.useDiffUV)
        diffuse = sun.diffuse * diff * vec3(texture(material.diffuseUV, UV));
    else
        diffuse = sun.diffuse.rgb * diff * material.diffuse;
    vec3 specular;
    if (material.useSpecUV)
        specular = sun.specular * spec * vec3(texture(material.specularUV, UV));
    else
        specular = sun.specular.rgb * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 ambient = vec3(1.0);
    if (material.useAmbiUV)
        ambient = light.ambient.rgb * vec3(texture(material.ambientUV, UV));
    else
        ambient = light.ambient.rgb * material.ambient;

    vec3 diffuse = vec3(1.0);
    if (material.useDiffUV)
        diffuse = light.diffuse.rgb * diff * vec3(texture(material.diffuseUV, UV));
    else
        diffuse = light.diffuse.rgb * diff * material.diffuse;

    vec3 specular = vec3(1.0);
    if (material.useSpecUV)
        specular = light.specular.rgb * spec * vec3(texture(material.specularUV, UV));
    else
        specular = light.specular.rgb * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

out vec4 color;

void main()
{

    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    result += CalcSunLight(sun, norm, viewDir);

    for (int i = 0; i < light_count; i++)
        result += CalcPointLight(lights[i], norm, FragPos, viewDir);

    color = vec4(result, 1.0);
}