#include "main.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <mutex>
#include <queue>
#include <stb_image/stb_image_write.h>

#include "window.h"
#include "camera.h"
#include "renderer.h"
#include "Object/Components/Custom/RotationComponent.h"
#include "Object/Components/Custom/SinWithOffsetYTranslationComponent.h"
#include "Object/Custom/SphereObject.h"
#include "Object/Custom/CubeObject.h" // Adicionada inclusão para CubeObject
#include "Object/Custom/Letters/AnyLetterObject.h"
#include "Object/Custom/Numbers/AnyNumberObject.h"
#include "Object/Meshes/Custom/Sphere.h"
#include "Utility/Constants/MathConsts.h"

bool RenderAnimation(const std::string& outputDir, int totalFrames, ViewMode viewMode) {
    // Criar diretório de saída se não existir
    std::filesystem::create_directories(outputDir);
    
    // Inicializar janela
    Window window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "CGAnimator");
    if (!window.initialize(false)) {
        std::cerr << "Falha ao inicializar janela" << '\n';
        return false;
    }
    
    // Inicializar renderer
    Renderer renderer(window);
    if (!renderer.initialize()) {
        std::cerr << "Falha ao inicializar renderer" << '\n';
        return false;
    }
    
    // Inicializar câmera
    Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));
    camera.SetObjectPosition(glm::vec3(0.5f, 0.0f, 10.0f));

    // EACH
    auto letterEObj = std::make_unique<AnyLetterObject>('E', Transform(-1.1f, 0.0f, 0.0f), Material());
    letterEObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f));
    auto letterAObj = std::make_unique<AnyLetterObject>('A', Transform(0.0f, 0.0f, 0.0f), Material());
    auto letterCObj = std::make_unique<AnyLetterObject>('C', Transform(1.15f, 0.0f, 0.0f), Material());
    auto letterHObj = std::make_unique<AnyLetterObject>('H', Transform(2.1f, 0.0f, 0.0f), Material());
    letterHObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f));

    // 20
    auto number2Obj = std::make_unique<AnyNumberObject>(2, Transform(-1.25f, -1.4f, 0.0f), Material());
    number2Obj->SetObjectScale(glm::vec3(0.9f, 0.9f, 1.0f) * 0.75f);
    number2Obj->SetObjectRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    auto number0Obj = std::make_unique<AnyLetterObject>('O', Transform(-0.6f, -1.35f, 0.0f), Material());
    number0Obj->SetObjectScale(glm::vec3(0.8f, 1.0f, 1.0f) * 0.75f);
    
    // ANOS
    auto letterAObj2 = std::make_unique<AnyLetterObject>('A', Transform(0.15f, -1.3f, 0.0f), Material());
    letterAObj2->SetObjectScale(glm::vec3(0.725f));
    auto letterNObj = std::make_unique<AnyLetterObject>('N', Transform(0.9f, -1.3f, 0.0f), Material());
    letterNObj->SetObjectScale(glm::vec3(0.75f));
    auto letterOObj = std::make_unique<AnyLetterObject>('O', Transform(1.65f, -1.3f, 0.0f), Material());
    letterOObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f) * 0.75f);
    auto letterSObj = std::make_unique<AnyLetterObject>('S', Transform(2.25f, -1.3f, 0.0f), Material("textures/magma.png"));
    letterSObj->SetObjectScale(glm::vec3(0.8f, 0.8f, 1.0f) * 0.75f);

    // Animação
    std::vector<std::unique_ptr<SinWithOffsetYTranslationComponent>> sinMovementComps;
    sinMovementComps.reserve(10);
    for (int i = 0; i < 10; ++i) {
        sinMovementComps.emplace_back(
            std::make_unique<SinWithOffsetYTranslationComponent>(0.5f, 0.125f)
        );
    }
    
    letterEObj->AddComponent(std::move(sinMovementComps[0]));
    letterAObj->AddComponent(std::move(sinMovementComps[1]));
    letterCObj->AddComponent(std::move(sinMovementComps[2]));
    letterHObj->AddComponent(std::move(sinMovementComps[3]));
    
    number2Obj->AddComponent(std::move(sinMovementComps[4]));
    number0Obj->AddComponent(std::move(sinMovementComps[5]));
    
    letterAObj2->AddComponent(std::move(sinMovementComps[6]));
    letterNObj->AddComponent(std::move(sinMovementComps[7]));
    letterOObj->AddComponent(std::move(sinMovementComps[8]));
    letterSObj->AddComponent(std::move(sinMovementComps[9]));

    // Adicionar a Cena
    
    Scene scene;
    scene.AddObjectToScene(std::move(letterEObj));
    scene.AddObjectToScene(std::move(letterAObj));
    scene.AddObjectToScene(std::move(letterCObj));
    scene.AddObjectToScene(std::move(letterHObj));

    scene.AddObjectToScene(std::move(number2Obj));
    scene.AddObjectToScene(std::move(number0Obj));
    
    scene.AddObjectToScene(std::move(letterAObj2));
    scene.AddObjectToScene(std::move(letterNObj));
    scene.AddObjectToScene(std::move(letterOObj));
    scene.AddObjectToScene(std::move(letterSObj));
    
    // Variáveis para controle de tempo
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    int frameIndex = 0;
    bool renderingComplete = false;
    
    // To Show FPS
    double lastTimeShowedFPS = glfwGetTime();
    int numOfFramesRenderedInLastSecond = 0;

    double startTime = glfwGetTime();

    // Loop principal
    while (!window.shouldClose())
    {
        // Calcular tempo delta
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        // Processar entrada do usuário
        window.processInput(deltaTime);
        
        switch (viewMode)
        {
        case ViewMode::RENDER_ONLY:
            frameIndex++;
            break;
        case ViewMode::INTERACTIVE:
            frameIndex++;
            break;
        default:
            std::cout << "Passe um modo de renderização válido ao executar usando \"-mode MODO\" no terminal. Modos são \"render\" e \"interactive\"." << std::endl;
            return false;
        }

        // Tickar a Scene
        scene.TickAll(deltaTime);
        
        // Atualizar janela e Renderizar
        renderer.renderFrame(camera, scene, deltaTime);
        numOfFramesRenderedInLastSecond++;
        
        // Mostrar o FPS na tela
        if (currentTime - lastTimeShowedFPS > 1.0f)
        {
            std::cout << "\rFPS: " << numOfFramesRenderedInLastSecond << std::flush;
            numOfFramesRenderedInLastSecond = 0;
            lastTimeShowedFPS = currentTime;
        }
    }
    
    return true;
}

int main(int argc, char* argv[]) {
    // Verificar argumentos de linha de comando
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int frames = TOTAL_FRAMES;
    std::string outputDir = OUTPUT_DIR;
    ViewMode viewMode = ViewMode::INTERACTIVE; // Modo interativo por padrão
    
    // Processar argumentos (se houver)
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "--width" && i + 1 < argc) {
                width = std::stoi(argv[++i]);
            }
            else if (arg == "--height" && i + 1 < argc) {
                height = std::stoi(argv[++i]);
            }
            else if (arg == "--frames" && i + 1 < argc) {
                frames = std::stoi(argv[++i]);
            }
            else if (arg == "--output" && i + 1 < argc) {
                outputDir = argv[++i];
            }
            else if (arg == "--mode" && i + 1 < argc) {
                std::string mode = argv[++i];
                if (mode == "interactive") {
                    viewMode = ViewMode::INTERACTIVE;
                } else if (mode == "render") {
                    viewMode = ViewMode::RENDER_ONLY;
                }
            }
            else if (arg == "--help") {
                std::cout << "Uso: " << argv[0] << " [opções]" << std::endl;
                std::cout << "Opções:" << std::endl;
                std::cout << "  --width N     Largura da janela (padrão: " << DEFAULT_WIDTH << ")" << std::endl;
                std::cout << "  --height N    Altura da janela (padrão: " << DEFAULT_HEIGHT << ")" << std::endl;
                std::cout << "  --frames N    Número total de frames (padrão: " << TOTAL_FRAMES << ")" << std::endl;
                std::cout << "  --output DIR  Diretório de saída (padrão: " << OUTPUT_DIR << ")" << std::endl;
                std::cout << "  --mode MODE   Modo de visualização (interactive/render, padrão: interactive)" << std::endl;
                std::cout << "  --help        Exibir esta ajuda" << std::endl;
                return 0;
            }
        }
    }
    
    // Renderizar animação
    if (!RenderAnimation(outputDir, frames, viewMode)) {
        std::cerr << "Falha ao renderizar animação" << std::endl;
        return 1;
    }
    
    return 0;
}
