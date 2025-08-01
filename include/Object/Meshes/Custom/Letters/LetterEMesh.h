#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterEMesh : public Mesh, protected CSGImplementable
{
public:
    LetterEMesh() = default;
    ~LetterEMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    float LetterEWithSDF(const glm::vec3& p);
};
