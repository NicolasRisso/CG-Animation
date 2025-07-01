#pragma once
#include "Object/SceneObject.h"

class AnyLetterObject : public SceneObject
{
public:
    AnyLetterObject(char letter);
    AnyLetterObject(char letter, const Transform& transform, const Material& material);
};