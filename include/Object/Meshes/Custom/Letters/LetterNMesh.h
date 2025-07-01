#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterNMesh : public Mesh, protected CSGImplementable
{
public:
    LetterNMesh() = default;
    ~LetterNMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    float LetterNWithSDF(const glm::vec3& p);
};
