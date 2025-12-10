struct PointLight {
    vec4 position;
    vec4 color;

    float constant;
    float linear;
    float quadratic;
    float padding;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform int light_count;

layout(std430, binding = 1) buffer PointLights {
    PointLight lights[];
};
