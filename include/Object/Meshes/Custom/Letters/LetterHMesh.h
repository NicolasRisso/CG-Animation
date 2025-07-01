#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterHMesh : public Mesh, protected CSGImplementable
{
public:
    LetterHMesh() = default;
    ~LetterHMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    float LetterHWithSDF(const glm::vec3& p);
};
