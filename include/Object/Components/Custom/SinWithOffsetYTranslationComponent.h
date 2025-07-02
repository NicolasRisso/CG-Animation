#pragma once

#include <glm/vec3.hpp>

#include "Object/Components/IComponent.h"

class SinWithOffsetYTranslationComponent : public IComponent
{
public:
    explicit SinWithOffsetYTranslationComponent(const float speed = 0.25f, const float amplitude = 1.0f)
    : m_Speed(speed), m_Amplitude(amplitude) {}

    void Start() override;
    void Tick(const float DeltaTime) override;

private:
    glm::vec3 m_OriginalPosition = glm::vec3(0.0f);
    float m_Speed = 0.25f;
    float m_Time = 0.0f;
    float m_Amplitude = 1.0f;
};