#version 460 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 inCenter; // per-instance
layout (location = 2) in float inScale; // per-instance

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec3 vPos;

uniform float u_time;
uniform mat4 model;

float simpleNoise(vec2 p) {
    return fract(sin(dot(p.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec3 pos = vertPos;

    // height displacement using simple noise
    float h = simpleNoise(pos.xz * 0.1) * 0.8;
    pos.y += h;
    

    // wind sway
    float sway = sin(u_time + pos.x * 0.5 + pos.z * 0.5) * 0.2;
    pos.x += sway;

    vPos = pos;

    vec3 worldPos = inCenter + pos * inScale;
	gl_Position = projection * view * model * vec4(worldPos, 1.0);
}