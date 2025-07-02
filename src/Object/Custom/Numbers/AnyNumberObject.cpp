#include "Object/Custom/Numbers/AnyNumberObject.h"

#include "Object/Meshes/Custom/Numbers/Number2Mesh.h"

AnyNumberObject::AnyNumberObject(const int number)
    : AnyNumberObject(number, Transform(), Material()) {}

AnyNumberObject::AnyNumberObject(const int number, const Transform& transform, const Material& material)
: SceneObject(transform, [&]()
{
    Mesh* numberMesh = nullptr;
    switch (number)
    {
        case 2: numberMesh = new Number2Mesh(); break;
        default: numberMesh = new Number2Mesh(); break;
    }
    
    numberMesh->initialize();
    return numberMesh;
}(), material){}