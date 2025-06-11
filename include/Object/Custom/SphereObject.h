#pragma once
#include "Object/SceneObject.h"

class SphereObject : public SceneObject
{
public:
    explicit SphereObject(unsigned int sectors = 36, unsigned int stacks = 12, float radius = 0.5f);
    SphereObject(unsigned int sectors, unsigned int stacks , float radius, const Transform& transform, const Material& material);
};
