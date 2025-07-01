#include "Object/Custom/Letters/AnyLetterObject.h"

#include "Object/Meshes/Custom/Letters/LetterCMesh.h"
#include "Object/Meshes/Custom/Letters/LetterEMesh.h"
#include "Object/Meshes/Custom/Letters/LetterHMesh.h"
#include "Object/Meshes/Custom/Letters/LetterOMesh.h"

AnyLetterObject::AnyLetterObject(const char letter)
    : AnyLetterObject(letter, Transform(), Material()) {}

AnyLetterObject::AnyLetterObject(const char letter, const Transform& transform, const Material& material)
: SceneObject(transform, [&]()
{
    char c = static_cast<char>(std::toupper(letter));
    Mesh* letterMesh = nullptr;
    switch (c)
    {
        case 'C': letterMesh = new LetterCMesh(); break;
        case 'E': letterMesh = new LetterEMesh(); break;
        case 'H': letterMesh = new LetterHMesh(); break;
        case 'O': letterMesh = new LetterOMesh(); break;
        default: letterMesh = new LetterCMesh(); break;
    }
    
    letterMesh->initialize();
    return letterMesh;
}(), material){}
