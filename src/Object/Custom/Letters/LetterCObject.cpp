#include "Object/Custom/Letters/LetterCObject.h"

#include "Object/Meshes/Custom/Letters/LetterCMesh.h"

LetterCObject::LetterCObject()
    : LetterCObject(Transform(), Material()) {}

LetterCObject::LetterCObject(const Transform& transform, const Material& material)
    : SceneObject(transform, [&]()
    {
        LetterCMesh* letterCMesh = new LetterCMesh();
        letterCMesh->initialize();
        return letterCMesh;
    }(), material){}
