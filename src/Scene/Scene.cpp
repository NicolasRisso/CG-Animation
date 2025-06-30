#include "Scene/Scene.h"

void Scene::StartAll()
{
    for (auto& [name, object] : m_Objects) object->Start();
}

void Scene::TickAll(const float DeltaTime)
{
    for (auto& [name, object] : m_Objects) object->Tick(DeltaTime);
}

bool Scene::AddObjectToScene(std::unique_ptr<SceneObject> object)
{
    auto [iterator, result] = m_Objects.emplace(object->GetName(), std::move(object));
    return result;
}

bool Scene::RemoveObjectFromScene(const SceneObject& object)
{
    return m_Objects.erase(object.GetName()) == 1;
}

bool Scene::RemoveObjectFromSceneByName(const std::string& name)
{
    auto iterator = m_Objects.find(name);
    if (iterator == m_Objects.end()) return false;
    
    m_Objects.erase(iterator);
    return true;
}

SceneObject* Scene::GetObjectFromScene(const std::string& name)
{
    const auto iterator = m_Objects.find(name);
    return iterator != m_Objects.end() ? iterator->second.get() : nullptr;
}

std::vector<SceneObject*> Scene::GetObjectsFromScene() const
{
    std::vector<SceneObject*> result;
    result.reserve(m_Objects.size());
    for (const auto& [key, object] : m_Objects) result.push_back(object.get());
    return result;
}
