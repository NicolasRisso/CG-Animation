#include "Object/Custom/Letters/LetterAObject.h"

#include "Object/Meshes/Custom/Letters/LetterAMesh.h"

LetterAObject::LetterAObject(const float width, const float height, const float barThickness, const float barHeightRatio)
    : LetterAObject(width, height, barThickness, barHeightRatio, Transform(), Material()){}

LetterAObject::LetterAObject(const float width, const float height, const float barThickness,
    const float barHeightRatio, const Transform& transform, const Material& material)
    : SceneObject(transform, ([&]()
    {
        LetterAMesh* letterAMesh = new LetterAMesh(width, height, barThickness, barHeightRatio);
        letterAMesh->initialize();
        return letterAMesh;
    })(), material){}
