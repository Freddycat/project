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

float min_intensity = 0.1;
float in_n = texture(noise, uv).r;
float n = mix(min_intensity, 1.0, in_n);
float n_left = texture(noise, uv - vec2(texel.x, 0)).r;
float n_right = texture(noise, uv + vec2(texel.x, 0)).r;
float n_down = texture(noise, uv - vec2(0, texel.y)).r;
float n_up = texture(noise, uv + vec2(0, texel.y)).r;

// black intensity:
// vec2 gradient = gradient_scale * vec2(n_left - n_right, n_down - n_up);
// white intensity:
// colors
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

  vec2 gradient = gradient_scale * vec2(n_right - n_left, n_up - n_down);

  if (length(gradient) < 0.0001) {
    gradient = vec2(0.0, 1.0); // default non-zero gradient
  }

  vec2 sway_direction_2D = -normalize(gradient);

  float sway_magnitude = length(gradient);

  float lean_angle = radians(80.0);

  float sway_angle = sway_magnitude * lean_angle;

  vec3 rotation_axis =
      normalize(vec3(sway_direction_2D.y, -sway_direction_2D.x, 0.0));

  float influence = 0.8; // tweak
  vec3 mixed_axis = normalize(mix(rotation_axis, scroll_axis, influence));

  // add a fix here to modulate the rotation?
  mat4 sway_mat = rotationMatrix(mixed_axis, sway_angle); // was forward
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
  color = vec4(shaded, 1.0);
}