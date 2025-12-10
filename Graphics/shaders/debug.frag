#version 460 core

out vec4 fColor;
in vec2 uv;
in vec4 color;

uniform sampler2D noise;

void main() {
    vec4 tex = texture(noise, uv);
    float intensity = tex.r;
    // fColor = vec4(uv, 0.0, 1.0);
    fColor = tex;
    // fColor = vec4(intensity, intensity, intensity, 1.0);
    // fColor = color;
}
