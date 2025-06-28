#pragma once

#include <memory>

#include "Core/Material.h"
#include "Core/Transform.h"
#include "Meshes/Mesh.h"

class SceneObject
{
public:
    SceneObject(Mesh* mesh, const Material& material);
    SceneObject(Transform transform, Mesh* mesh, const Material& material);
    SceneObject(const std::string& name, Mesh* mesh, const Material& material);
    SceneObject(const std::string& name, Transform transform, Mesh* mesh, const Material& material);

    void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const std::vector<Light>& lights) const;

    // Name Getters and Setters
    [[nodiscard]] std::string GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    
    // Core Getters and Setters
    [[nodiscard]] Transform GetTransform() const { return m_Transform; }
    [[nodiscard]] Material GetMaterial() const { return m_Mesh->getMaterial(); }
    [[nodiscard]] Mesh* GetMesh() const { return m_Mesh.get(); }

    void SetTransform(const Transform& transform) { m_Transform = transform; }
    void SetMaterial(const Material& material) const { m_Mesh->setMaterial(material); }
    void SetMesh(Mesh* rawMesh) { m_Mesh.reset(rawMesh); }

    // Transform Related Getters
    [[nodiscard]] glm::vec3 GetObjectPosition() const { return m_Transform.position; }
    [[nodiscard]] glm::vec3 GetObjectRotation() const { return m_Transform.rotation; }
    [[nodiscard]] glm::vec3 GetObjectScale() const { return m_Transform.scale; }

    void SetObjectPosition(const glm::vec3& position) { m_Transform.position = position; }
    void SetObjectRotation(const glm::vec3& rotation) { m_Transform.rotation = rotation; }
    void SetObjectScale(const glm::vec3& scale) { m_Transform.scale = scale; }
    
private:
    std::string m_Name;
    
    Transform m_Transform;
    std::unique_ptr<Mesh> m_Mesh;
};
