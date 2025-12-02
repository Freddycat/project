#version 460 core

layout (location = 0) in vec3 vertPos;
layout(location = 1) in vec3 inCenter;
layout (location = 2) in vec3 inSize;
layout (location = 3) in vec4 inColor;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec4 color;

void main() {
    // determine if vertex is top/bottom hemisphere or cylinder
    float z = vertPos.z;
    vec3 scaled = vertPos;
    float radius = inSize.x * 0.5;

    if (z > 0.0) {             // top hemisphere
        scaled.xy *= inSize.xy;
        scaled.z = inSize.z*0.5 + scaled.z * inSize.x - inSize.x; 
    } else if (z < 0.0) {      // bottom hemisphere
        scaled.xy *= inSize.xy;
        scaled.z = -inSize.z*0.5 + scaled.z * inSize.x + inSize.x;
    }

    vec3 worldPos = inCenter + scaled;
    gl_Position = projection * view * vec4(worldPos, 1.0);
    color = inColor;
}