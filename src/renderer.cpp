#include "optimization.h"
#include "renderer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "Object/SceneObject.h"
#include "Utility/Constants/EngineLimits.h"

// Implementação otimizada do Renderer

Renderer::Renderer(Window& window)
    : m_window(window)
{
    setupLights();
}

Renderer::~Renderer()
{
    // Cleanup será feito pelos destrutores das classes membros
}

bool Renderer::initialize()
{
    // Habilitar teste de profundidade
    glEnable(GL_DEPTH_TEST);
    
    // Habilitar blending para transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Otimizações OpenGL
    glEnable(GL_CULL_FACE);  // Eliminar faces não visíveis
    glCullFace(GL_BACK);     // Culling de faces traseiras

    setupLights();
    
    return true;
}

void Renderer::renderFrame(const Camera& camera, SceneObject& object, const float deltaTime)
{
    // Limpar buffer
    m_window.clear(0.05f, 0.05f, 0.05f, 1.0f);
    
    // Atualizar posições das luzes
    updateLights(deltaTime);

    object.Draw(camera.getViewMatrix(), camera.getProjectionMatrix(m_window.getAspectRatio()), camera.getPosition(), m_lights);
    
    m_window.update();
}

bool Renderer::saveFrameToImage(const std::string& outputPath, const int frameNumber)
{
    // Usar monitor de performance para medir tempo de salvamento
    static PerformanceMonitor perfMonitor;
    perfMonitor.startOperation("frame_save");
    
    // Obter dimensões da janela
    int width = m_window.getWidth();
    int height = m_window.getHeight();
    
    // Usar buffer otimizado para pixels
    MemoryManager::Buffer<unsigned char> pixels(width * height * 4);
    
    // Ler pixels do framebuffer
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    
    // Criar nome do arquivo
    std::stringstream ss;
    ss << outputPath << "/frame_" << std::setw(5) << std::setfill('0') << frameNumber << ".png";
    std::string filename = ss.str();
    
    // Salvar imagem usando stb_image_write (implementação será adicionada)
    // Por enquanto, apenas simular o salvamento
    std::cout << "Salvando frame " << frameNumber << " em " << filename << std::endl;
    
    // Registrar tempo de salvamento
    double saveTime = perfMonitor.endOperation("frame_save");
    
    // A cada 60 frames, imprimir estatísticas de desempenho
    if (frameNumber % 60 == 0) {
        std::cout << "Frame " << frameNumber << " salvo em " 
                  << saveTime << " ms" << std::endl;
    }
    
    return true;
}

void Renderer::updateLights(float deltaTime)
{
    // Atualizar posições das luzes com base no tempo
    m_accumulateTime += deltaTime;
    const float time = m_accumulateTime; // Assumindo 60 FPS
    
    // Luz 0: Movimento vertical de cima para baixo
    m_lights[0].position.y = 2.0f + sin(time * 1.5f) * 1.5f;
    
    // Luz 1: Movimento circular ao redor do cubo
    constexpr float radius = 2.0f;
    m_lights[1].position.x = sin(time) * radius;
    m_lights[1].position.z = cos(time) * radius;
    
    // Luz 2: Movimento vertical oposto à luz 1
    m_lights[2].position.y = 2.0f + sin(time * 1.5f + 3.14159f) * 1.5f;
    
    // Luz 3: Movimento circular em fase oposta à luz 2
    m_lights[3].position.x = sin(time + 3.14159f) * radius;
    m_lights[3].position.z = cos(time + 3.14159f) * radius;
    
    // Atualizar cores das luzes
    // Luz 1: Transição de vermelho para amarelo
    m_lights[0].color = glm::vec3(
        1.0f,
        0.5f + 0.5f * sin(time * 0.7f),
        0.0f
    );
    
    // Luz 2: Transição de azul para ciano
    m_lights[1].color = glm::vec3(
        0.0f,
        0.5f + 0.5f * sin(time * 0.9f),
        1.0f
    );
    
    // Luz 3: Transição de verde para amarelo
    m_lights[2].color = glm::vec3(
        0.5f + 0.5f * sin(time * 0.8f),
        1.0f,
        0.0f
    );
    
    // Luz 4: Transição de roxo para rosa
    m_lights[3].color = glm::vec3(
        0.8f,
        0.0f,
        0.5f + 0.5f * sin(time * 1.1f)
    );
}

void Renderer::setLightColor(const int lightIndex, const glm::vec3& color)
{
    if (lightIndex >= 0 && lightIndex < EngineLimits::MAX_LIGHTS)
    {
        m_lights[lightIndex].color = color;
    }
}

void Renderer::setLightPosition(const int lightIndex, const glm::vec3& position)
{
    if (lightIndex >= 0 && lightIndex < EngineLimits::MAX_LIGHTS)
    {
        m_lights[lightIndex].position = position;
    }
}

bool Renderer::combineFramesToVideo(const std::string& framesPath, const std::string& outputPath, 
                                   int frameCount, int fps)
{
    // Usar monitor de performance para medir tempo de combinação
    static PerformanceMonitor perfMonitor;
    perfMonitor.startOperation("video_combine");
    
    // Esta função será implementada usando FFmpeg ou outra biblioteca
    // Por enquanto, apenas simular a combinação
    std::cout << "Combinando " << frameCount << " frames em " << outputPath 
              << " com " << fps << " FPS" << std::endl;
    
    // Registrar tempo de combinação
    double combineTime = perfMonitor.endOperation("video_combine");
    std::cout << "Vídeo combinado em " << combineTime << " ms" << std::endl;
    
    return true;
}

void Renderer::setupLights()
{
    m_lights.resize(EngineLimits::MAX_LIGHTS);

    // Configurar posições iniciais das luzes
    m_lights[0].position = glm::vec3(0.0f, 2.0f, 0.0f);
    m_lights[1].position = glm::vec3(2.0f, 0.0f, 0.0f);    // Direita
    m_lights[2].position = glm::vec3(0.0f, -2.0f, 0.0f);   // Base
    m_lights[3].position = glm::vec3(-2.0f, 0.0f, 0.0f);   // Esquerda
    
    // Configurar cores iniciais das luzes
    m_lights[0].color = glm::vec3(1.0f, 0.5f, 0.0f);       // Laranja
    m_lights[1].color = glm::vec3(0.0f, 0.5f, 1.0f);       // Azul claro
    m_lights[2].color = glm::vec3(0.5f, 1.0f, 0.0f);       // Verde claro
    m_lights[3].color = glm::vec3(0.8f, 0.0f, 0.5f);       // Roxo

    // Resto das configurações da luz pegam o default da struct.
}