#include "Object/Custom/CubeObject.h"
#include "Object/Meshes/Custom/Cube.h" // Para criar a new Cube()

// Construtor de conveniência
CubeObject::CubeObject(const Material& material)
    : CubeObject(Transform(), material) { // Chama o construtor principal com uma Transform padrão
}

// Construtor principal
CubeObject::CubeObject(const Transform& transform, const Material& material)
    : SceneObject(transform, ([&]() {
        Cube* cubeMesh = new Cube();
        cubeMesh->initialize(); // Inicializa a Mesh (VAO, VBO, EBO, etc.)
        return cubeMesh;
    })(), material) {
    // O corpo do construtor pode ficar vazio se nenhuma lógica adicional for necessária.
    // SceneObject cuidará de definir o material na Mesh.
}
