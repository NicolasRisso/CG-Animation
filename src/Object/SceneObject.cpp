#include "Object/SceneObject.h"

static int s_NextID = 0;

SceneObject::SceneObject(Mesh* mesh, const Material& material)
    : m_Transform(0.0f, 0.0f, 0.0f), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
    m_Name = "SceneObject" + std::to_string(++s_NextID);
}

SceneObject::SceneObject(Transform transform, Mesh* mesh, const Material& material)
    : m_Transform(transform), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
    m_Name = "SceneObject" + std::to_string(++s_NextID);
}

SceneObject::SceneObject(const std::string& name, Mesh* mesh, const Material& material)
    : m_Transform(0.0f, 0.0f, 0.0f), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
    m_Name = name;
}

SceneObject::SceneObject(const std::string& name, Transform transform, Mesh* mesh, const Material& material)
    : m_Transform(transform), m_Mesh(mesh)
{
    m_Mesh->setMaterial(material);
    m_Name = name;
}

void SceneObject::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition,
                       const std::vector<Light>& lights) const
{
    m_Mesh->render(viewMatrix, projectionMatrix, m_Transform.getModelMatrix(), lights, cameraPosition);
}

void SceneObject::AddComponent(std::unique_ptr<IComponent> component)
{
    component->SetOwner(this);
    component->Start();
    m_Components.push_back(std::move(component));
}

void SceneObject::Start() const
{
    for (const auto& component : m_Components) component->Start();
}

void SceneObject::Tick(const float DeltaTime) const
{
    for (const auto& component : m_Components) component->Tick(DeltaTime);
}
