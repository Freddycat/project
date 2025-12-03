#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "Graphics.h"
#include "camera.h"

#include "global.h"

#include <stb/stb_image.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void LoadNoise(GLuint &shader)
{
    auto filePath = g.home / "shaders" / "perlinNoise.png";
    const std::string path_string = filePath.string();
    const char *path = path_string.c_str();
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
        {
            std::cout << "RED" << std::endl;

            format = GL_RED; // Grayscale
        }
        else if (nrChannels == 3)
        {
            std::cout << "RGB" << std::endl;

            format = GL_RGB; // Standard color
        }
        else if (nrChannels == 4)
        {
            std::cout << "RGBA" << std::endl;

            format = GL_RGBA; // Color with alpha
        }
        else
        {
            std::cout << "DEFAULT" << std::endl;

            // Handle error or default to GL_RGB
            format = GL_RGB;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glUseProgram(shader);

    // bind texture to texture unit 0 once
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // connect sampler uniform to unit 0 once
    glUniform1i(glGetUniformLocation(shader, "noise"), 0);

    GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
}

using glm::vec3;

void BindShape(GLuint &buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Gfx::AddVertexAttrib(1, 3, sizeof(Shape), offsetof(Shape, center));
    glVertexAttribDivisor(1, 1);
    Gfx::AddVertexAttrib(2, 3, sizeof(Shape), offsetof(Shape, size));
    glVertexAttribDivisor(2, 1);
    Gfx::AddVertexAttrib(3, 4, sizeof(Shape), offsetof(Shape, color));
    glVertexAttribDivisor(3, 1);
}

void BindShapeSym(GLuint &buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    Gfx::AddVertexAttrib(1, 3, sizeof(ShapeSym), offsetof(ShapeSym, center));
    glVertexAttribDivisor(1, 1);
    Gfx::AddVertexAttrib(2, 1, sizeof(ShapeSym), offsetof(ShapeSym, size));
    glVertexAttribDivisor(2, 1);
    Gfx::AddVertexAttrib(3, 4, sizeof(ShapeSym), offsetof(ShapeSym, color));
    glVertexAttribDivisor(3, 1);
}

void SetupShaders(Graphics &graphics, Gizmos &gizmos, Camera &camera)
{
    LoadBasics();

    gizmos.points.reserve(1000);
    gizmos.static_lines.reserve(1000);
    gizmos.lines.reserve(1000);

    gizmos.triangles.reserve(10000);
    gizmos.grass.reserve(10000);
    gizmos.circles.reserve(50);
    gizmos.cubes.reserve(50);
    gizmos.plane.reserve(64);

    graphics.max_points = std::max<size_t>(256, gizmos.points.capacity());
    graphics.max_line_points_static = std::max<size_t>(1024, gizmos.static_lines.capacity());

    graphics.max_triangles = std::max<size_t>(2048, gizmos.triangles.capacity());

    graphics.max_circles = std::max<size_t>(64, gizmos.circles.capacity());
    graphics.max_cubes_wireframe = std::max<size_t>(64, gizmos.wireframe_cubes.capacity());
    graphics.max_cubes = std::max<size_t>(64, gizmos.cubes.capacity());
    graphics.max_spheres = std::max<size_t>(64, gizmos.spheres.capacity());
    graphics.max_ground = std::max<size_t>(64, gizmos.plane.capacity());
    graphics.max_grass = std::max<size_t>(5000, gizmos.grass.capacity());

    std::filesystem::path shaderpath = g.home / "shaders";

    // Create all the shader programs:
    GLuint vert = Gfx::InitializeShader(shaderpath / "vert.glsl", shaderpath / "frag.glsl");
    GLuint vert_inst_size = Gfx::InitializeShader(shaderpath / "vert_inst_size.glsl", shaderpath / "frag.glsl");
    GLuint vert_inst_scale = Gfx::InitializeShader(shaderpath / "vert_inst_scale.glsl", shaderpath / "frag.glsl");
    GLuint vert_ground = Gfx::InitializeShader(shaderpath / "vert_ground.glsl", shaderpath / "frag_ground.glsl");
    GLuint vert_grass = Gfx::InitializeShader(shaderpath / "grass2.vert", shaderpath / "frag.glsl");
    GLuint vert_debug = Gfx::InitializeShader(shaderpath / "v_debug.glsl", shaderpath / "f_debug.glsl");

    graphics.shaders[SHADER_VERTS] = vert;
    graphics.shaders[SHADER_SIZE] = vert_inst_size;
    graphics.shaders[SHADER_SCALE] = vert_inst_scale;
    graphics.shaders[SHADER_GROUND] = vert_ground;
    graphics.shaders[SHADER_GRASS] = vert_grass;
    graphics.shaders[SHADER_DEBUG] = vert_debug;

    LoadNoise(graphics.shaders[SHADER_GRASS]);
    LoadNoise(graphics.shaders[SHADER_DEBUG]);

    Gfx::CheckGLError("Gfx::Use"); // this is just an error check, not sure if it works.

    // ----------------------------------------------
    // Create a global camera "uniform buffer" to Use
    glGenBuffers(1, &graphics.cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, graphics.cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_DYNAMIC_DRAW); // allocates the 2 mat4
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, graphics.cameraUBO);                       // binds to "0" from layout
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // --------------------
    // -- POINTS (VERTS) --

    GLuint pointAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(pointAttrib);
    GLuint pointBuffer = Gfx::CreateVBO(graphics.max_points * sizeof(Point), gizmos.points.data(), GL_DYNAMIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(Point), offsetof(Point, pos));   // pos
    Gfx::AddVertexAttrib(1, 4, sizeof(Point), offsetof(Point, color)); // color
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_POINTS] = pointAttrib;
    graphics.buffers[BUFF_POINTS] = pointBuffer;

    // -----------
    // -- LINES --

    GLuint lineAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(lineAttrib);
    GLuint lineBuffer = Gfx::CreateVBO(graphics.max_lines * sizeof(Point), gizmos.lines.data(), GL_DYNAMIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(Point), offsetof(Point, pos));   // pos
    Gfx::AddVertexAttrib(1, 4, sizeof(Point), offsetof(Point, color)); // color
    Gfx::UnbindVAO();
    // static
    GLuint lineAttribStatic = Gfx::CreateVAO();
    Gfx::UseVAO(lineAttribStatic);
    GLuint lineBufferStatic = Gfx::CreateVBO(graphics.max_lines * sizeof(Point), gizmos.lines.data(), GL_DYNAMIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(Point), offsetof(Point, pos));   // pos
    Gfx::AddVertexAttrib(1, 4, sizeof(Point), offsetof(Point, color)); // color
    Gfx::UnbindVAO();

    graphics.buffers[BUFF_LINES_STATIC] = lineBufferStatic;
    graphics.attribs[BUFF_LINES_STATIC] = lineAttribStatic;
    graphics.buffers[BUFF_LINES] = lineBuffer;
    graphics.attribs[BUFF_LINES] = lineAttrib;

    // ---------------------
    // -- WIREFRAME CUBES --
    /*
    GLuint wireframeCubeAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(wireframeCubeAttrib);
    GLuint wireframeCubeBuffer = Gfx::CreateVBO(base_cube_wireframe.size() * sizeof(vec3), base_cube_wireframe.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);
    GLuint wireframeCubeInstance = Gfx::CreateVBO(gizmos.wireframe_cubes.size() * sizeof(ShapeSym), nullptr, GL_DYNAMIC_DRAW);
    BindShapeSym(wireframeCubeInstance);
    Gfx::UnbindVAO();

    graphics.attribs[WIREFRAME_CUBES] = wireframeCubeAttrib;
    graphics.buffers[WIREFRAME_CUBES] = wireframeCubeInstance;
 */
    // ---------------------
    // -- SOLID CUBES --
    GLuint cubeAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(cubeAttrib);
    GLuint cubeBuffer = Gfx::CreateVBO(base_cube.size() * sizeof(vec3), base_cube.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);
    GLuint cubeInstance = Gfx::CreateVBO(graphics.max_cubes * sizeof(ShapeSym), nullptr, GL_DYNAMIC_DRAW);
    BindShapeSym(cubeInstance);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_CUBES] = cubeAttrib;
    graphics.buffers[BUFF_CUBES] = cubeInstance;

    // -- CIRCLES --
    GLuint circleAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(circleAttrib);
    GLuint circleBuffer = Gfx::CreateVBO(base_circle.size() * sizeof(vec3), base_circle.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);
    GLuint circleInstance = Gfx::CreateVBO(graphics.max_circles * sizeof(ShapeSym), nullptr, GL_DYNAMIC_DRAW);
    BindShapeSym(circleInstance);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_CIRCLES] = circleAttrib;
    graphics.buffers[BUFF_CIRCLES] = circleInstance;

    // -- SPHERES --

    GLuint sphereAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(sphereAttrib);
    GLuint sphereBuffer = Gfx::CreateVBO(base_sphere.size() * sizeof(vec3), base_sphere.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);
    GLuint sphereInstance = Gfx::CreateVBO(graphics.max_spheres * sizeof(ShapeSym), nullptr, GL_DYNAMIC_DRAW);
    BindShapeSym(sphereInstance);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_SPHERES] = sphereAttrib;
    graphics.buffers[BUFF_SPHERES] = sphereInstance;

    // -- CAPSULES --

    GLuint capsuleAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(capsuleAttrib);
    GLuint capsuleBuffer = Gfx::CreateVBO(base_capsule.size() * sizeof(vec3), base_capsule.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0); // vert pos
    GLuint capsuleInstance = Gfx::CreateVBO(graphics.max_capsules * sizeof(Shape), nullptr, GL_DYNAMIC_DRAW);
    BindShape(capsuleInstance);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_CAPSULES] = capsuleAttrib;
    graphics.buffers[BUFF_CAPSULES] = capsuleInstance;

    // ------------
    // -- GROUND --
    /*
        GLuint groundAttrib = Gfx::CreateVAO();
        Gfx::UseVAO(groundAttrib);
        GLuint groundBuffer = Gfx::CreateVBO(base_ground.size() * sizeof(vec3), base_ground.data(), GL_STATIC_DRAW);

        Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);
        GLuint groundInstance = Gfx::CreateVBO(graphics.max_ground * sizeof(Shape), nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, groundInstance);

        Gfx::AddVertexAttrib(1, 2, sizeof(Plane), offsetof(Ground, uv));
        glVertexAttribDivisor(1, 1);
        Gfx::UnbindVAO();

        graphics.attribs[BUFF_GROUND] = groundAttrib;
        graphics.buffers[BUFF_GROUND] = groundInstance;
     */
    // ------------
    // -- DEBUG --

    GLuint debugAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(debugAttrib);
    GLuint debugBuffer = Gfx::CreateVBO(base_plane.size() * sizeof(Vertex_Basic), base_plane.data(), GL_STATIC_DRAW);
    Gfx::AddVertexAttrib(0, 3, sizeof(Vertex_Basic), offsetof(Vertex_Basic, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_Basic), (void *)offsetof(Vertex_Basic, uv));

    GLuint debugInstance = Gfx::CreateVBO(graphics.max_ground * sizeof(Plane), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, debugInstance);

    auto v4s = sizeof(glm::vec4);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Plane), (void *)offsetof(Plane, transform));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Plane), (void *)(offsetof(Plane, transform) + 1 * v4s));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Plane), (void *)(offsetof(Plane, transform) + 2 * v4s));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Plane), (void *)(offsetof(Plane, transform) + 3 * v4s));

    // glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_DEBUG] = debugAttrib;
    graphics.buffers[BUFF_DEBUG] = debugInstance;

    // -----------
    // -- GRASS --

    GLuint grassAttrib = Gfx::CreateVAO();
    Gfx::UseVAO(grassAttrib);
    GLuint grassBuffer = Gfx::CreateVBO(base_triangle.size() * sizeof(vec3), base_triangle.data(), GL_STATIC_DRAW);
    // vertPos
    Gfx::AddVertexAttrib(0, 3, sizeof(vec3), 0);

    GLuint grassInstance = Gfx::CreateVBO(graphics.max_grass * sizeof(GrassBlade), nullptr, GL_DYNAMIC_DRAW);
    // pos
    glBindBuffer(GL_ARRAY_BUFFER, grassInstance);
    Gfx::AddVertexAttrib(1, 3, sizeof(GrassBlade), offsetof(GrassBlade, pos));
    glVertexAttribDivisor(1, 1);
    // size
    Gfx::AddVertexAttrib(2, 3, sizeof(GrassBlade), offsetof(GrassBlade, size));
    glVertexAttribDivisor(2, 1);
    // color
    Gfx::AddVertexAttrib(3, 4, sizeof(GrassBlade), offsetof(GrassBlade, color));
    glVertexAttribDivisor(3, 1);
    Gfx::UnbindVAO();

    graphics.attribs[BUFF_GRASS] = grassAttrib;
    graphics.buffers[BUFF_GRASS] = grassInstance;
    //------------------
    // -- END SHADERS --

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0f);

    for (auto &shader : graphics.shaders)
    {
        GLint success;

        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error here: 0x" << std::hex << err << std::dec << std::endl;
    }
};