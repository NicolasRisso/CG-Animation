#include "Object/Components/Custom/SinWithOffsetYTranslationComponent.h"

#include "Object/SceneObject.h"

void SinWithOffsetYTranslationComponent::Start()
{
    IComponent::Start();

    m_OriginalPosition = GetOwner()->GetObjectPosition();
    m_Time = 0.0f;
}

void SinWithOffsetYTranslationComponent::Tick(const float DeltaTime)
{
    IComponent::Tick(DeltaTime);

    m_Time += DeltaTime;

    const float offsetY = m_Amplitude * std::sin(m_Time * m_Speed + m_OriginalPosition.x);
    const glm::vec3 currentPosition = GetOwner()->GetObjectPosition();
    const glm::vec3 newPosition =  + glm::vec3(currentPosition.x, m_OriginalPosition.y + offsetY, currentPosition.z);
    GetOwner()->SetObjectPosition(newPosition);
}
