#pragma once
#include <string>
#include <unordered_map>

#include "Object/SceneObject.h"

class Scene
{
public:
    /** Tries adding given SceneObject to the Scene.
     * If they were already an Object with this name returns false. */
    bool AddObjectToScene(SceneObject object);

    /** Tries removing a given object from the scene. */
    bool RemoveObjectFromScene(const SceneObject& object);

    /** Tries removing a given object from the scene by its name. */
    bool RemoveObjectFromSceneByName(const std::string& name);

    /** Returns object in the scene with give name. */
    SceneObject* GetObjectFromScene(const std::string& name);

    /** Return all objects from the scene. */
    std::vector<SceneObject> GetObjectsFromScene() const;

private:
    std::pmr::unordered_map<std::string, SceneObject> m_Objects;
};
