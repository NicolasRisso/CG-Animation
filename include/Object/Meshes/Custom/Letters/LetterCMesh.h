#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterCMesh : public Mesh, protected CSGImplementable
{
public:
    LetterCMesh() = default;
    ~LetterCMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    float LetterCWithSDF(const glm::vec3& p);
};