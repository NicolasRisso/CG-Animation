#include "Scene/Scene.h"

bool Scene::AddObjectToScene(SceneObject object)
{
    auto [iterator, result] = m_Objects.emplace(std::move(object.GetName()), std::move(object));
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
    auto iterator = m_Objects.find(name);
    return iterator != m_Objects.end() ? &iterator->second : nullptr;
}

std::vector<SceneObject> Scene::GetObjectsFromScene() const
{
    std::vector<SceneObject> result;
    result.reserve(m_Objects.size());
    for (auto& [key, object] : m_Objects) result.push_back(object);
    return result;
}
