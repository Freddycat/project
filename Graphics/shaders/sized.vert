
#version 460 core
#include "camera.glsl"

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNorm;
layout (location = 2) in vec2 vertUV;
layout (location = 3) in vec3 inCenter;
layout (location = 4) in vec3 inScale;
layout (location = 5) in vec4 inColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;
out vec4 VertColor;

void main() {
  vec3 WorldPos = inCenter + vertPos * inScale;
    Normal = normalize(vertNorm);  
    UV = vertUV;
    FragPos = WorldPos;
    VertColor = inColor;
	gl_Position = projection * view * vec4(WorldPos, 1.0);
}