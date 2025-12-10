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

out vec4 VertColor;
uniform vec3 characterPos;

uniform sampler2D noise;
uniform vec2 scroll = vec2(0.10, 0.10);
uniform float time;
uniform float noise_frequency = 1.0; // Zoom
uniform float noise_speed_x = 0.01;  // Scroll speed
uniform float noise_speed_y = 0.01;  // Scroll speed
uniform float gradient_scale = 1.0;  // Strength of bending

vec2 new_scroll = vec2(scroll.x * noise_speed_x, scroll.y *noise_speed_y);
vec3 scroll_axis =
    normalize(cross(vec3(0, 0, 1), vec3(new_scroll.x, new_scroll.y, 0.0)));

vec2 uv = (inBase.xy / 500.0) * noise_frequency + time * new_scroll;

vec2 texel = 1.0 / textureSize(noise, 0);

// 1. Sample procedural noise
float windNoiseDir = texture(noise, uv).r * 2.0 - 1.0; // [-1,1]
// 2. Remap to angle
float windAngle = windNoiseDir * 6.2831853; // [-2π,2π]

float windNoiseStrength =
    texture(noise, uv + vec2(13.1, 7.3)).r * 2.0 - 1.0; // [-1,1]

// 3. Optional easing
float lean = pow(abs(windNoiseStrength), 2.0) * 0.25 * sign(windNoiseStrength);

// 4. Convert to rotation axis
vec3 axis = vec3(sin(windAngle), 0.0, cos(windAngle));

vec3 base = inColor.rgb;        // bottom color
vec3 tip = vec3(0.8, 1.0, 0.4); // top color
vec3 shaded = mix(base, tip, vertPos.z);

void main() {

  vec3 local = vertPos * inSize;

  vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]);
  vec3 camUp = vec3(view[0][1], view[1][1], view[2][1]);

  vec3 worldOffset =
      camRight * local.x + camUp * local.y + vec3(0.0, 0.0, local.z);

  local = worldOffset;
/* 
  // 1. Rotate to face camera
  float angle = radians(45);
  mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
  local.xy = rot * local.xy; */

  vec3 up = normalize(local);
  vec3 forward = normalize(vec3(1.0, 1.0, 0.0));
  vec3 right = normalize(cross(forward, up));

  float influence = 0.5; // tweak

  vec3 rotation_axis = normalize(mix(axis, scroll_axis, influence));
  mat4 sway_mat = rotationMatrix(rotation_axis, lean);
  local = (sway_mat * vec4(local, 1.0)).xyz;

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
  VertColor = vec4(shaded, 1.0);
}