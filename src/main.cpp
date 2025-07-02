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
#include "Object/Custom/SphereObject.h"
#include "Object/Custom/CubeObject.h" // Adicionada inclusão para CubeObject
#include "Object/Custom/Letters/AnyLetterObject.h"
#include "Object/Custom/Numbers/AnyNumberObject.h"
#include "Object/Meshes/Custom/Sphere.h"
#include "Utility/Constants/MathConsts.h"

// Shared save queue for frames
std::queue<std::pair<std::shared_ptr<std::vector<unsigned char>>, std::string>> g_saveQueue;
std::mutex                                     g_queueMutex;
std::condition_variable                        g_queueCV;
static bool                                           g_savingActive = false;
static std::thread                                    g_saverThread;

// Worker that writes PNGs in background
void SaveWorker(const int width, const int height) {
    while (g_savingActive || !g_saveQueue.empty()) {
        std::unique_lock<std::mutex> lk(g_queueMutex);
        g_queueCV.wait(lk, []{
            return !g_savingActive || !g_saveQueue.empty();
        });
        while (!g_saveQueue.empty()) {
            auto [pixels, filename] = std::move(g_saveQueue.front());
            g_saveQueue.pop();
            lk.unlock();
            stbi_write_png(
                filename.c_str(), width, height, 4,
                pixels->data(), width * 4);
            lk.lock();
        }
    }
}

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
    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
    
    // Inicializar SceneObjects
    // auto sphereObj = std::make_unique<SphereObject>(128, 32, 0.5f, Transform(-1.0f, 0.0f, 0.0f), Material());
    //
    // auto cubeObj = std::make_unique<CubeObject>(Transform(0.0f, 0.0f, 0.0f), Material());
    // cubeObj->AddComponent(std::make_unique<RotationComponent>(glm::vec3(90.f, 0.f, 0.f)));
    // auto cubeObj1 = std::make_unique<CubeObject>(Transform(1.0f, 0.0f, 0.0f), Material());

    // EACH
    auto letterEObj = std::make_unique<AnyLetterObject>('E', Transform(-1.1f, 0.0f, 0.0f), Material());
    letterEObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f));
    auto letterAObj = std::make_unique<AnyLetterObject>('A', Transform(0.0f, 0.0f, 0.0f), Material());
    auto letterCObj = std::make_unique<AnyLetterObject>('C', Transform(1.15f, 0.0f, 0.0f), Material());
    auto letterHObj = std::make_unique<AnyLetterObject>('H', Transform(2.1f, 0.0f, 0.0f), Material());
    letterHObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f));

    // 20
    auto number2Obj = std::make_unique<AnyNumberObject>(2, Transform(-1.1f, -1.35f, 0.0f), Material());
    number2Obj->SetObjectScale(glm::vec3(0.9f, 0.9f, 1.0f) * 0.7f);
    number2Obj->SetObjectRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    auto number0Obj = std::make_unique<AnyLetterObject>('O', Transform(-0.5f, -1.3f, 0.0f), Material());
    number0Obj->SetObjectScale(glm::vec3(0.7f, 1.0f, 1.0f) * 0.75f);
    
    // ANOS
    auto letterAObj2 = std::make_unique<AnyLetterObject>('A', Transform(0.1f, -1.3f, 0.0f), Material());
    letterAObj2->SetObjectScale(glm::vec3(0.75f));
    auto letterNObj = std::make_unique<AnyLetterObject>('N', Transform(0.7f, -1.3f, 0.0f), Material());
    letterNObj->SetObjectScale(glm::vec3(0.75f));
    auto letterOObj = std::make_unique<AnyLetterObject>('O', Transform(1.3f, -1.3f, 0.0f), Material());
    letterOObj->SetObjectScale(glm::vec3(0.9f, 1.0f, 1.0f) * 0.75f);
    auto letterSObj = std::make_unique<AnyLetterObject>('S', Transform(1.9f, -1.3f, 0.0f), Material());
    letterSObj->SetObjectScale(glm::vec3(0.8f, 0.8f, 1.0f) * 0.75f);

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

    renderer.startRecording("captures/", 30);

    if (viewMode == ViewMode::RENDER_ONLY)
    {
        // start background saver
        g_savingActive = true;
        g_saverThread  = std::thread(SaveWorker, window.getWidth(), window.getHeight());
        renderer.startRecording(outputDir, 30);
    }

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
        
        if (viewMode == ViewMode::RENDER_ONLY && frameIndex++ >= totalFrames)
        {
            // shutdown saver
            g_saverThread.join();
            std::string videoPath = outputDir + "/animation.mp4";
            renderer.combineFramesToVideo(videoPath);
            break;
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
