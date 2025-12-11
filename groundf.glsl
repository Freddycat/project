#version 460 core
out vec4 FragColor;
in vec3 vPos;

void main()
{
    // simple grass color gradient
    vec3 color = mix(vec3(0.1, 0.5, 0.1), vec3(0.2, 0.8, 0.2), vPos.y * 2.0);
    FragColor = vec4(color, 1.0);
}
