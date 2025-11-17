#version 460 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 inCenter;
layout (location = 2) in vec4 inSize;
layout (location = 3) in vec4 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 fColor;

void main() {
    // determine if vertex is top/bottom hemisphere or cylinder
    float z = aPos.z;
    vec3 scaled = aPos;
    float radius = inSize.x * 0.5;

    if (z > 0.0) {             // top hemisphere
        scaled.xy *= inSize.xy;
        scaled.z = inSize.z*0.5 + scaled.z * inSize.x - inSize.x; 
    } else if (z < 0.0) {      // bottom hemisphere
        scaled.xy *= inSize.xy;
        scaled.z = -inSize.z*0.5 + scaled.z * inSize.x + inSize.x;
    }

    vec3 worldPos = inCenter.xyz + scaled;
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
    fColor = aColor.xyz;
}