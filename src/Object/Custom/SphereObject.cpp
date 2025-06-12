#include "Object/Custom/SphereObject.h"

#include "Object/SceneObject.h"
#include "Object/Meshes/Custom/Sphere.h"

SphereObject::SphereObject(const unsigned int sectors, unsigned int stacks, float radius)
    : SphereObject(sectors, stacks, radius, Transform{0.0f, 0.0f, 0.0f}, Material()){}

SphereObject::SphereObject(unsigned int sectors, unsigned int stacks, float radius, const Transform& transform, const Material& material)
    : SceneObject(transform, new Sphere(sectors, stacks, radius), material){}
