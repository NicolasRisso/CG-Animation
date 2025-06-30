#include "Object/Components/Custom/RotationComponent.h"

#include "Object/SceneObject.h"

void RotationComponent::Tick(const float DeltaTime)
{
    glm::vec3 newRotation = GetOwner()->GetObjectRotation();

    newRotation += m_RotationSpeed * DeltaTime;
    GetOwner()->SetObjectRotation(newRotation);
}
