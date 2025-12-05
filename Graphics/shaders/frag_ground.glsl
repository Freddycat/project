#version 460 core
in vec4 color;
in vec3 vPos;

out vec4 FragColor;

void main()
{
    //vec3 color = mix(vec3(0.1, 0.5, 0.1), vec3(0.2, 0.8, 0.2), vPos.z * 2.0);
    FragColor = color;
}
