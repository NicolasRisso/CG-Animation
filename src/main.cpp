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

#include "window.h"
#include "camera.h"
#include "renderer.h"
#include "Object/Custom/SphereObject.h"
#include "Object/Custom/CubeObject.h" // Adicionada inclusão para CubeObject
#include "Object/Meshes/Custom/Sphere.h"
#include "Utility/Constants/MathConsts.h"

bool renderAnimation(const std::string& outputDir, int totalFrames, ViewMode viewMode) {
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
    
    // Inicializar cubo
    // SphereObject sphereObj = SphereObject(128, 32, 0.5f, Transform(0.0f, 0.0f, 0.0f), Material("textures/ShaderToyTextures/Abstract1.jpg"));
    CubeObject cubeObj(Transform(0.0f, 0.0f, 0.0f), Material("textures/ShaderToyTextures/Abstract1.jpg"));
    
    // Configurar posição inicial do cubo
    cubeObj.SetObjectPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    cubeObj.SetObjectScale(glm::vec3(1.0f, 1.0f, 1.0f));
    
    // Variáveis para controle de tempo
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    int frameIndex = 0;
    bool renderingComplete = false;

    // Movement Variables
    const glm::vec3 originalScale = cubeObj.GetObjectScale();
    
    // To Show FPS
    double lastTimeShowedFPS = glfwGetTime();
    int numOfFramesRenderedInLastSecond = 0;
    
    // Loop principal
    while (!window.shouldClose())
    {
        // Calcular tempo delta
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        // Processar entrada do usuário
        window.processInput(deltaTime);
        
        // Atualizar rotação do cubo
        constexpr float rotationSpeed = 45.0f;  // 45 graus por segundo
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        rotation.y += rotationSpeed * deltaTime;
        rotation.x += rotationSpeed * deltaTime * 0.5f;
        cubeObj.SetObjectRotation(cubeObj.GetObjectRotation() + rotation);
        //cube.render(shader, camera.getViewMatrix(), camera.getProjectionMatrix(window.getWidth() / window.getHeight()));

        // Movimenta a sphere pro lado
        constexpr float movementAmplitude = 0.5f;
        constexpr float movementFrequency = 1.0f;

        const float offsetX = movementAmplitude * sinf(2.0f * MathConstants::PI * movementFrequency * currentTime);
        
        glm::vec3 newPosition = glm::vec3(0.0f, offsetX, 1.0f);
        cubeObj.SetObjectPosition(newPosition);

        // Sphere Scale
        constexpr float scaleAmplitude = 0.5f;
        constexpr float scaleFrequency = 0.25f;

        const float scaleOffSet = scaleAmplitude * sinf(2.0f * MathConstants::PI * scaleFrequency * currentTime);
        glm::vec3 newScale = originalScale + glm::vec3(scaleOffSet);
        cubeObj.SetObjectScale(newScale);
        
        switch (viewMode)
        {
        case ViewMode::RENDER_ONLY:
            if (!renderingComplete && frameIndex < totalFrames)
            {
                // Salvar frame como imagem
                if (viewMode == ViewMode::RENDER_ONLY)
                {
                    if (!renderer.saveFrameToImage(outputDir, frameIndex))
                    {
                        std::cerr << "Falha ao salvar frame " << frameIndex << std::endl;
                        return false;
                    }
                    // Exibir progresso
                    float progress = static_cast<float>(frameIndex + 1) / static_cast<float>(totalFrames) * 100.0f;
                    std::cout << "Progresso: " << std::fixed << std::setprecision(1) << progress << "%" << std::endl;
                }
                frameIndex++;
            }
            // Verificar se a renderização foi concluída
            else
            {
                renderingComplete = true;
                
                // Combinar frames em vídeo
                std::string outputVideo = outputDir + "/animation.mp4";
                if (!renderer.combineFramesToVideo(outputDir, outputVideo, totalFrames, 60)) {
                    std::cerr << "Falha ao combinar frames em vídeo" << std::endl;
                    return false;
                }
                
                std::cout << "Animação renderizada com sucesso: " << outputVideo << std::endl;
                
                // No modo de renderização apenas, sair do loop
                if (viewMode == ViewMode::RENDER_ONLY) {
                    std::cout << "todos os frames renderizados, saindo do loop\n";
                    break;
                }
                
                std::cout << "Renderização concluída. Pressione ESC para sair ou continue interagindo com a visualização." << std::endl;
            }
            break;
        case ViewMode::INTERACTIVE:
            frameIndex++;
            break;
        default:
            std::cout << "Passe um modo de renderização válido ao executar usando \"-mode MODO\" no terminal. Modos são \"render\" e \"interactive\"." << std::endl;
            return false;
        }
        
        // Atualizar janela e Renderizar
        renderer.renderFrame(camera, cubeObj, deltaTime);
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
    if (!renderAnimation(outputDir, frames, viewMode)) {
        std::cerr << "Falha ao renderizar animação" << std::endl;
        return 1;
    }
    
    return 0;
}
