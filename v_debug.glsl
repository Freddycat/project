#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 inUv;
layout(location = 2) in mat4 transform;

layout(std140, binding = 0) uniform Camera {
  mat4 projection;
  mat4 view;
};

out vec2 uv;
out vec4 color;

uniform vec2 noiseScale = vec2(1); // how much to "zoom"

void main() {

  gl_Position = projection * view * transform * vec4(pos, 1.0);
  color = vec4(0.5, 0.5, 0.5, 1);
  uv = inUv * noiseScale;
}
