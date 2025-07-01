#pragma once
#include "Object/Meshes/CSGImplementable.h"
#include "Object/Meshes/Mesh.h"

class LetterAMesh : public Mesh, protected CSGImplementable
{
public:
    LetterAMesh() = default;
    ~LetterAMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
    
private:
    float LetterAWithSDF(const glm::vec3& p);
};