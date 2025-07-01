#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterOMesh : public Mesh, protected CSGImplementable
{
public:
    LetterOMesh() = default;
    ~LetterOMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    void LetterCWithSDF(const glm::vec3& p);
};
