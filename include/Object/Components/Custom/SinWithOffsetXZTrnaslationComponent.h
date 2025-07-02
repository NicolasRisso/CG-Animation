#pragma once

#include <glm/vec3.hpp>

#include "camera.h"
#include "Object/Components/IComponent.h"

class SinWithOffsetXZTrnaslationComponent : public IComponent
{
public:
    explicit SinWithOffsetXZTrnaslationComponent(const float speed = 0.25f, const float amplitudeX = 1.0f, const float amplitudeZ = 0.1f)
    : m_Speed(speed), m_AmplitudeX(amplitudeX), m_AmplitudeZ(amplitudeZ) {}

    void Start() override;
    void Tick(const float DeltaTime) override;

private:
    Camera* m_Camera = nullptr;
    
    glm::vec3 m_OriginalPosition = glm::vec3(0.0f);
    glm::vec3 m_InitialDir = glm::vec3(0.0f);
    glm::vec3 m_LookAt = glm::vec3(0.0f);
    
    float m_Speed = 0.25f;
    float m_Time = 0.0f;
    float m_AmplitudeX = 1.0f;
    float m_AmplitudeZ = 0.1f;
};