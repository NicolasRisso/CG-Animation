#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterSMesh : public Mesh, protected CSGImplementable
{
public:
    LetterSMesh() = default;
    ~LetterSMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
    
private:
    float LetterSWithSDF(const glm::vec3& p);
};