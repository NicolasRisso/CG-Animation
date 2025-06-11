#pragma once
#include <algorithm>
#include <memory>

#include "Core/Material.h"
#include "Core/Transform.h"
#include "Meshes/Mesh.h"

class SceneObject
{
public:
    SceneObject(Mesh* mesh, Material material);
    SceneObject(Transform transform, Mesh* mesh, Material material);

    void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const std::vector<Light>& lights);
    
    // Core Getters and Setters
    [[nodiscard]] Transform GetTransform() const { return m_Transform; }
    [[nodiscard]] Material GetMaterial() const { return m_Material; }
    [[nodiscard]] Mesh* GetMesh() const { return m_Mesh.get(); }

    void SetTransform(const Transform& transform) { m_Transform = transform; }
    void SetMaterial(const Material& material) { m_Material = material; }
    void SetMesh(Mesh* rawMesh) { m_Mesh.reset(rawMesh); }

    // Transform Related Getters
    [[nodiscard]] glm::vec3 GetObjectPosition() const { return m_Transform.position; }
    [[nodiscard]] glm::vec3 GetObjectRotation() const { return m_Transform.rotation; }
    [[nodiscard]] glm::vec3 GetObjectScale() const { return m_Transform.scale; }

    void SetObjectPosition(const glm::vec3& position) { m_Transform.position = position; }
    void SetObjectRotation(const glm::vec3& rotation) { m_Transform.rotation = rotation; }
    void SetObjectScale(const glm::vec3& scale) { m_Transform.scale = scale; }

protected:
    const Material c_DefaultSphereMaterial = Material("Shaders/default.vs", "Shaders/default.fs","textures/metal_texture.jpg");
    
private:
    Transform m_Transform;
    std::unique_ptr<Mesh> m_Mesh;
    Material m_Material;
};
