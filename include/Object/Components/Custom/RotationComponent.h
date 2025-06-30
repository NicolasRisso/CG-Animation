#pragma once

#include <glm/vec3.hpp>

#include "Object/Components/IComponent.h"

class RotationComponent : public IComponent
{
public:
    explicit RotationComponent(const glm::vec3 rotationSpeed) : m_RotationSpeed(rotationSpeed) {}
    
    void Tick(const float DeltaTime) override;

private:
    glm::vec3 m_RotationSpeed = glm::vec3(1.0f);
};