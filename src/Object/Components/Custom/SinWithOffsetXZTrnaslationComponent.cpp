#include "Object/Components/Custom/SinWithOffsetXZTrnaslationComponent.h"

void SinWithOffsetXZTrnaslationComponent::Start()
{
    IComponent::Start();

    m_Camera = static_cast<Camera*>(GetOwner());
    if (!m_Camera) return;
    
    m_OriginalPosition = m_Camera->GetObjectPosition();
    m_InitialDir = m_Camera->getFront();

    m_LookAt = m_OriginalPosition + m_InitialDir;
    m_Time = 0.0f;
}

void SinWithOffsetXZTrnaslationComponent::Tick(const float DeltaTime)
{
    IComponent::Tick(DeltaTime);

    m_Time += DeltaTime;

    const float offX = m_AmplitudeX * std::sin(m_Time * m_Speed + 1);
    const float sinZ = std::sin(m_Time * m_Speed + 1);
    const float offZ = m_AmplitudeZ * (sinZ * 0.5f + 0.5f);

    const glm::vec3 newPos = m_OriginalPosition - glm::vec3(offX, 0.0f, offZ);
    m_Camera->SetObjectPosition(newPos);

    const glm::vec3 dir = glm::normalize(m_LookAt - newPos);

    const float yawRad = std::atan2(dir.z, dir.x);
    const float pitchRad = std::asin(dir.y);

    const float yawDeg = glm::degrees(yawRad);
    const float pitchDeg = glm::degrees(pitchRad);

    //m_Camera->SetCameraRotation(glm::vec3(yawDeg, pitchDeg, 0.0f));
}
