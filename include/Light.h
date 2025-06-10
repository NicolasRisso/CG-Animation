#pragma once
#include <glm/glm.hpp>

/** Dados da luz para passar para um shader. */
struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};