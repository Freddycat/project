#version 460 core
#extension GL_ARB_shading_language_include : enable
#include "camera.glsl"

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNorm;
layout(location = 2) in vec2 vertUV;
layout(location = 3) in mat4 transform;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

void main()
{
    vec4 WorldPos = transform * vec4(vertPos, 1.0);
    Normal = normalize(mat3(transform) * vertNorm);
    UV = vertUV;
    FragPos = WorldPos.xyz;

    gl_Position = projection * view * WorldPos;
}