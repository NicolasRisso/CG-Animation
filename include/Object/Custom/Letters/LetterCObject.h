#pragma once
#include "Object/SceneObject.h"

class LetterCObject : public SceneObject
{
public:
    LetterCObject();
    LetterCObject(const Transform& transform, const Material& material);
};