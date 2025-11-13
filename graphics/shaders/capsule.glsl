#version 460 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 inCenter;
layout (location = 2) in vec4 inSize;
layout (location = 3) in vec3 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 fColor;

void main() {
    vec3 scaled = aPos * inSize; // scale position by w/h
    vec3 worldPos = inCenter + scaled; // center position
	gl_Position = uProjection * uView * vec4(worldPos, 1.0);
	fColor = aColor;
}