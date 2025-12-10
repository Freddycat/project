#include <iostream>
#include "World.h"
#include "drawFunctions.h"

namespace Draw {
namespace Lights {

void DrawLights(
    Graphics &graphics,
    Gizmos &gizmos,
    Camera &camera)
{

    auto &shader = graphics.shaders[SHADER_SIZED];
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, graphics.lightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, graphics.lightSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                    gizmos.lights.size() * sizeof(Light), gizmos.lights.data());

    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera.position));

    GLint light_count_uni = glGetUniformLocation(graphics.shaders[SHADER_SIZED], "light_count");
    if (light_count_uni == -1)
        std::cerr << "Warning: light_count uniform not found\n";
    else {
        glUniform1i(light_count_uni, gizmos.lights.size());
    }

    auto &sun = gizmos.sun;
    glUniform3fv(glGetUniformLocation(shader, "sun.direction"), 1, glm::value_ptr(sun.direction));
    glUniform3fv(glGetUniformLocation(shader, "sun.ambient"), 1, glm::value_ptr(sun.ambient));
    glUniform3fv(glGetUniformLocation(shader, "sun.diffuse"), 1, glm::value_ptr(sun.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "sun.specular"), 1, glm::value_ptr(sun.specular));
}

} // namespace Lights
} // namespace Draw