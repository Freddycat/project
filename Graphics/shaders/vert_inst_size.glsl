#version 460 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 inCenter;
layout (location = 2) in float inScale;
layout (location = 3) in vec4 inColor;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec4 color;

void main() {
  vec3 worldPos = inCenter + vertPos * inScale;
	gl_Position = projection * view * vec4(worldPos, 1.0);
	color = inColor;
}