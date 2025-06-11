#include "Object/SceneObject.h"

SceneObject::SceneObject(Mesh* mesh, Material material)
    : m_Mesh(mesh), m_Material(std::move(material)), m_Transform(0.0f, 0.0f, 0.0f) {}

SceneObject::SceneObject(Transform transform, Mesh* mesh, Material material)
    : m_Transform(transform), m_Mesh(mesh), m_Material(std::move(material)) {}

void SceneObject::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition,
                       const std::vector<Light>& lights)
{
    m_Material.bind();

    m_Material.shader.setMat4("model", m_Transform.getModelMatrix());
    m_Material.shader.setMat4("view", viewMatrix);
    m_Material.shader.setMat4("projection", projectionMatrix);
    m_Material.shader.setVec3("viewPosition", cameraPosition);

    m_Mesh->render(viewMatrix, projectionMatrix, m_Transform.getModelMatrix(), lights, cameraPosition);
}
