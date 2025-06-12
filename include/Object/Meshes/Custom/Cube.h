#pragma once

#include "Object/Meshes/Mesh.h"
#include <vector>

class Cube : public Mesh {
public:
    Cube();
    ~Cube() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
};
