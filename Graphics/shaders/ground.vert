#version 460 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in mat4 inUv;
layout (location = 2) in mat4 transform;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec2 uv;
out vec4 color;
out vec3 vPos;

float simpleNoise(vec2 p) {
    return fract(sin(dot(p.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec3 pos = vertPos;
    
    color = vec4(0.1, 0.5, 0.1, 1.0);

    // height displacement using simple noise
    float h = simpleNoise(pos.xz * 0.1) * 0.1;
    pos.z += h;
    
    vPos = pos;

	gl_Position = projection * view * transform * vec4(vertPos, 1.0);
}