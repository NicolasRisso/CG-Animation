#include "Object/Custom/Letters/AnyLetterObject.h"

#include "Object/Meshes/Custom/Letters/LetterAMesh.h"
#include "Object/Meshes/Custom/Letters/LetterCMesh.h"
#include "Object/Meshes/Custom/Letters/LetterEMesh.h"
#include "Object/Meshes/Custom/Letters/LetterHMesh.h"
#include "Object/Meshes/Custom/Letters/LetterNMesh.h"
#include "Object/Meshes/Custom/Letters/LetterOMesh.h"
#include "Object/Meshes/Custom/Letters/LetterSMesh.h"

AnyLetterObject::AnyLetterObject(const char letter)
    : AnyLetterObject(letter, Transform(), Material()) {}

AnyLetterObject::AnyLetterObject(const char letter, const Transform& transform, const Material& material)
: SceneObject(transform, [&]()
{
    const char c = static_cast<char>(std::toupper(letter));
    Mesh* letterMesh = nullptr;
    switch (c)
    {
        case 'A': letterMesh = new LetterAMesh(); break;
        case 'C': letterMesh = new LetterCMesh(); break;
        case 'E': letterMesh = new LetterEMesh(); break;
        case 'H': letterMesh = new LetterHMesh(); break;
        case 'N': letterMesh = new LetterNMesh(); break;
        case 'O': letterMesh = new LetterOMesh(); break;
        case 'S': letterMesh = new LetterSMesh(); break;
        default: letterMesh = new LetterCMesh(); break;
    }
    
    letterMesh->initialize();
    return letterMesh;
}(), material){}
