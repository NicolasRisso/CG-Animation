#include "Object/SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, const Material& material)
    : m_Transform(0.0f, 0.0f, 0.0f), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
}

SceneObject::SceneObject(Transform transform, Mesh* mesh, const Material& material)
    : m_Transform(transform), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
}

void SceneObject::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition,
                       const std::vector<Light>& lights) const
{
    m_Mesh->render(viewMatrix, projectionMatrix, m_Transform.getModelMatrix(), lights, cameraPosition);
}
