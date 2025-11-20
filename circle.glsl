#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inCenter; // per-instance
layout (location = 2) in float inScale; // per-instance
layout (location = 3) in vec3 aColor;

// uniform mat4 uProjection;
// uniform mat4 uView;

out vec3 fColor;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
};

void main() {
  vec3 worldPos = inCenter + aPos * inScale;
	gl_Position = proj * view * vec4(worldPos, 1.0);
	fColor = aColor;
}