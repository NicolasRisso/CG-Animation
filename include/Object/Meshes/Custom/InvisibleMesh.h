#pragma once
#include "Object/Meshes/Mesh.h"

class InvisibleMesh : public Mesh
{
public:
    InvisibleMesh() = default;
    ~InvisibleMesh() override = default;
    
protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
};