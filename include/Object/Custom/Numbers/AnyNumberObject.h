#pragma once
#include "Object/SceneObject.h"

class AnyNumberObject : public SceneObject
{
public:
    AnyNumberObject(const int number);
    AnyNumberObject(const int number, const Transform& transform, const Material& material);
};