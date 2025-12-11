#version 460 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 inBase;
layout(location = 2) in vec3 inSize;
layout(location = 3) in vec4 inColor;

layout(std140, binding = 0) uniform Camera {
  mat4 projection;
  mat4 view;
};

mat4 rotationMatrix(vec3 axis, float angle) {
  axis = normalize(axis);
  float s = sin(angle);
  float c = cos(angle);
  float oc = 1.0 - c;

  return mat4(
      oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s,
      oc * axis.z * axis.x + axis.y * s, 0.0, oc * axis.x * axis.y + axis.z * s,
      oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
      oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s,
      oc * axis.z * axis.z + c, 0.0, 0.0, 0.0, 0.0, 1.0);
}

out vec4 color;

uniform float time;
uniform vec3 characterPos;
uniform vec2 noiseScroll = vec2(-0.00, 0.0);
uniform sampler2D noise;

float t = time * 1;

vec2 uv = inBase.xy * 1 + t * noiseScroll;
// (uv = texture pos)
float n_in = texture(noise, uv).r; // (n = intensity)
float n = 1.0 - n_in;

vec2 texel = 1.0 / textureSize(noise, 0);

const float OFFSET = 1;

float n_left = texture(noise, uv - vec2(texel.x, 0.0)).r;
float n_right = texture(noise, uv + vec2(texel.x, 0.0)).r;
float n_down = texture(noise, uv - vec2(0.0, texel.y)).r;
float n_up = texture(noise, uv + vec2(0.0, texel.y)).r;

float n_x = n_right - n_left;
float n_y = n_up - n_down;

//float n_x = 1.0 - intensity_x;
//float n_y = 1.0 - intensity_y;

vec2 gradient = vec2(n_x, n_y);
// The Wind/Sway Vector points AWAY from the high-intensity area
vec2 sway_direction_2D = -normalize(gradient);
float sway_magnitude = length(gradient);
// Use the magnitude of the change to set the force.

float phase = n * 6.28318; // 0–2π

vec3 base = inColor.rgb;        // bottom color
vec3 tip = vec3(0.8, 1.0, 0.4); // top color
vec3 shaded = mix(base, tip, vertPos.z);

void main() {

  vec3 local = vertPos * inSize;

  // 1. Rotate to face camera
  float angle = radians(45);
  mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
  local.xy = rot * local.xy;

  vec3 up = normalize(local);
  vec3 forward = normalize(vec3(1.0, 1.0, 0.0));
  vec3 right = normalize(cross(forward, up));

  // Define a maximum bias in radians (e.g., 5 degrees)
  //float max_angle = radians(90.0);

  // Interpolate the bias angle:
  // n=0 (high noise) -> bias = -max_bias (sway left)
  // n=1 (low noise) -> bias = +max_bias (sway right)

  // float bias_angle = mix(-max_bias * 0.1, max_bias, n);
  //  Original:
  //  float swayangle = sin(t + phase) * radians(8.0) * strength;

  // Modified: Add the bias_angle
  // float sway_range = radians(8.0) * strength; // Calculate max displacement
  // float swayangle = sin(t + phase) * sway_range + bias_angle; // Apply bias

  // float swayangle = sway_magnitude * max_angle;

  float max_lean_angle =
      radians(80.0); // Use a realistic max lean, e.g., 15 deg


  float swayangle =
      clamp(sway_magnitude * max_lean_angle * 10.0, 0.0, max_lean_angle);
  vec3 rotation_axis =
      normalize(vec3(sway_direction_2D.y, -sway_direction_2D.x, 0.0));

      /* 
  mat4 swaymat = rotationMatrix(rotation_axis, swayangle); // was forward
  local = (swaymat * vec4(local, 1.0)).xyz;
 */

float testAngle = radians(90.0);
mat4 testRot = rotationMatrix(rotation_axis, testAngle);
local = (testRot * vec4(local, 1.0)).xyz;

  vec3 toChar = inBase - characterPos;

  // vec2 dir = normalize(inBase.xy - characterPos.xy);
  float dist = length(toChar);
  float bendFactor = clamp((90.0 - dist) / 90.0, 0.0, 1.0);
  float bendangle = bendFactor * 0.5;

  if (dist < 26.0) {
    vec3 dir = normalize(toChar);
    vec3 axis = normalize(cross(dir, up));
    mat4 char_rot = rotationMatrix(axis, bendangle);
    local = (char_rot * vec4(local, 1.0)).xyz;
  }

  vec3 worldPos = inBase + local;

  gl_Position = projection * view * vec4(worldPos, 1.0);
  color = vec4(shaded, 1.0);
}