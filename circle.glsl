#version 460 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 inCenter; // per-instance
layout (location = 2) in float inScale; // per-instance
layout (location = 3) in vec3 inColor;

// uniform mat4 uProjection;
// uniform mat4 uView;

out vec3 fColor;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

void main() {
  vec3 worldPos = inCenter + vertPos * inScale;
	gl_Position = proj * view * vec4(worldPos, 1.0);
	fColor = inColor;
}