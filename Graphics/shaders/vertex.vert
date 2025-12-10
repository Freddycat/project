#version 460 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec4 inColor;

layout(std140, binding = 0) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec4 VertColor;

void main() {
	gl_Position = projection * view * vec4(vertPos, 1.0);
	VertColor = inColor;
}