#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout(location = 2) in vec3 inCenter; // per-instance
layout(location = 3) in float inScale; // per-instance

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 fColor;

void main() {
  vec3 worldPos = inCenter + aPos * inScale;
	gl_Position = uProjection * uView * vec4(worldPos, 1.0);
	fColor = aColor;
}