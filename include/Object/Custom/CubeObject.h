#pragma once

#include "Object/SceneObject.h"
#include "Object/Core/Material.h" // Necessário para o construtor de conveniência
#include "Object/Core/Transform.h" // Necessário para o construtor de conveniência

class CubeObject : public SceneObject {
public:
    explicit CubeObject(const Material& material);
    CubeObject(const Transform& transform, const Material& material);
    ~CubeObject() override = default;
};
