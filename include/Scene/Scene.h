#pragma once

#include <string>
#include <unordered_map>

#include "Object/SceneObject.h"

class Scene
{
public:
    void StartAll();
    void TickAll(const float DeltaTime);
    
    /** Tries adding given SceneObject to the Scene.
     * If they were already an Object with this name returns false. */
    bool AddObjectToScene(std::unique_ptr<SceneObject> object);

    /** Tries removing a given object from the scene. */
    bool RemoveObjectFromScene(const SceneObject& object);

    /** Tries removing a given object from the scene by its name. */
    bool RemoveObjectFromSceneByName(const std::string& name);

    /** Returns object in the scene with give name. */
    SceneObject* GetObjectFromScene(const std::string& name);

    /** Return all objects from the scene. */
    std::vector<SceneObject*> GetObjectsFromScene() const;

private:
    std::unordered_map<std::string, std::unique_ptr<SceneObject>> m_Objects;
};
