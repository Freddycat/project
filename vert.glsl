#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// uniform mat4 uProjection;
// uniform mat4 uView;

out vec3 fColor;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 proj;
};

void main() {
	gl_Position = proj * view * vec4(aPos, 1.0);
	fColor = aColor;
}