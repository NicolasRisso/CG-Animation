#pragma once
#include "Object/SceneObject.h"

class LetterAObject : public SceneObject
{
public:
    LetterAObject(const float width = 1.0f, const float height = 1.0f, const float barThickness = 0.1f, const float barHeightRatio = 0.5f);
    LetterAObject(const float width, const float height, const float barThickness, const float barHeightRatio, const Transform& transform, const Material& material);
};