#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class Number2Mesh : public Mesh, protected CSGImplementable
{
public:
    Number2Mesh() = default;
    ~Number2Mesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
    
private:
    float Number2WithSDF(const glm::vec3& p);
};