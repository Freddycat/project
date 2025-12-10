#include <iostream>
#include "primitives.h"
#include "drawFunctions.h"

namespace Draw {
namespace Shapes {

void SetMaterial(GLuint &shader, Material &mat)
{
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(mat.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(mat.specular));
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(mat.ambient));
    glUniform1f(glGetUniformLocation(shader, "material.shine"), mat.shine);
    glUniform1i(glGetUniformLocation(shader, "material.useAmbiUV"), false);
    glUniform1i(glGetUniformLocation(shader, "material.useDiffUV"), false);
    glUniform1i(glGetUniformLocation(shader, "material.useSpecUV"), false);
}

void DrawCubes(Graphics &graphics, Gizmos &gizmos, Camera &camera)
{

    auto &shader = graphics.shaders[SHADER_SIZED];
    Gfx::Use(graphics.shaders[SHADER_SIZED]);

    Material mat;

    SetMaterial(shader, mat);

    Lights::DrawLights(graphics, gizmos, camera);

    Gfx::UseVAO(graphics.attribs[BUFF_CUBES]);
    if (!gizmos.cubes.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CUBES]);

        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        gizmos.cubes.size() * sizeof(Shape),
                        gizmos.cubes.data());
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_cube.size(),
                          gizmos.cubes.size());
    Gfx::UnbindVAO();

    if (!gizmos.wireframe_cubes.empty()) {

        auto &shader = graphics.shaders[SHADER_GIZMO];
        Gfx::Use(shader);

        Gfx::UseVAO(graphics.attribs[BUFF_CUBES]);

        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_CUBES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        gizmos.wireframe_cubes.size() * sizeof(Shape),
                        gizmos.wireframe_cubes.data());

        glDrawArraysInstanced(GL_LINES, 0, base_cube.size(),
                              gizmos.wireframe_cubes.size());

        Gfx::UnbindVAO();
    }
}

void DrawSpheres(Graphics &graphics, Gizmos &gizmos, Camera &camera)
{

    auto &shader = graphics.shaders[SHADER_SIZED];
    Gfx::Use(graphics.shaders[SHADER_SIZED]);

    Material mat;

    SetMaterial(shader, mat);

    Lights::DrawLights(graphics, gizmos, camera);

    Gfx::UseVAO(graphics.attribs[BUFF_SPHERES]);

    if (!gizmos.spheres.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, graphics.buffers[BUFF_SPHERES]);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        gizmos.spheres.size() * sizeof(Shape),
                        gizmos.spheres.data());
    } else {
        std::cout << "Spheres are empty!" << std::endl;
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, base_sphere.size(),
                          gizmos.spheres.size());
    Gfx::UnbindVAO();
}

} // namespace Shapes
} // namespace Draw