#include "Object/Custom/SphereObject.h"

#include "Object/SceneObject.h"
#include "Object/Meshes/Custom/Sphere.h"

SphereObject::SphereObject(const unsigned int sectors, unsigned int stacks, float radius)
    : SphereObject(sectors, stacks, radius, Transform{0.0f, 0.0f, 0.0f}, Material()){}

SphereObject::SphereObject(unsigned int sectors, unsigned int stacks, float radius, const Transform& transform, const Material& material)
    : SceneObject(transform, ([&]() {
        Sphere* sphereMesh = new Sphere(sectors, stacks, radius);
        // Note que m_Material ainda não foi definido aqui, então a Mesh usará seu material padrão para inicializar.
        // Isso será corrigido quando SceneObject chamar setMaterial.
        // O importante é que os buffers e VAO sejam criados.
        sphereMesh->initialize();
        return sphereMesh;
    })(), material)
{}
