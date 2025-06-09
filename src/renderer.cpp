#include "optimization.h"
#include "renderer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

// Implementação otimizada do Renderer

Renderer::Renderer(Window& window)
    : m_window(window), m_shader("shaders/cube.vs", "shaders/cube.fs")
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
    
    return true;
}

void Renderer::renderFrame(Camera& camera, Cube& cube, float deltaTime, int currentFrame)
{
    // Usar monitor de performance para medir tempo de renderização
    static PerformanceMonitor perfMonitor;
    perfMonitor.startOperation("frame_render");
    
    // Limpar buffer
    m_window.clear(0.05f, 0.05f, 0.05f, 1.0f);
    
    // Atualizar posições das luzes
    updateLights(deltaTime, currentFrame);
    
    // Configurar shader
    m_shader.use();
    
    // Configurar propriedades do material
    m_shader.setFloat("material.shininess", 64.0f);
    
    // Configurar luzes
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        std::string lightName = "lights[" + std::to_string(i) + "]";
        m_shader.setVec3(lightName + ".position", m_lightPositions[i]);
        m_shader.setVec3(lightName + ".color", m_lightColors[i]);
        
        // Configurar atenuação
        m_shader.setFloat(lightName + ".constant", 1.0f);
        m_shader.setFloat(lightName + ".linear", 0.09f);
        m_shader.setFloat(lightName + ".quadratic", 0.032f);
    }
    
    // Configurar posição da câmera
    m_shader.setVec3("viewPos", camera.getPosition());
    
    // Renderizar cubo
    cube.render(m_shader, camera.getViewMatrix(), 
                camera.getProjectionMatrix(m_window.getAspectRatio()));
    
    // Registrar tempo de renderização
    double renderTime = perfMonitor.endOperation("frame_render");
    
    // A cada 60 frames, imprimir estatísticas de desempenho
    if (currentFrame % 60 == 0) {
        std::cout << "Frame " << currentFrame << " renderizado em " 
                  << renderTime << " ms" << std::endl;
    }
}

bool Renderer::saveFrameToImage(const std::string& outputPath, int frameNumber)
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

void Renderer::updateLights(float deltaTime, int currentFrame)
{
    // Atualizar posições das luzes com base no tempo
    float time = static_cast<float>(currentFrame) / 60.0f; // Assumindo 60 FPS
    
    // Luz 1: Movimento vertical de cima para baixo
    m_lightPositions[0].y = 2.0f + sin(time * 1.5f) * 1.5f;
    
    // Luz 2: Movimento circular ao redor do cubo
    float radius = 2.0f;
    m_lightPositions[1].x = sin(time) * radius;
    m_lightPositions[1].z = cos(time) * radius;
    
    // Luz 3: Movimento vertical oposto à luz 1
    m_lightPositions[2].y = 2.0f + sin(time * 1.5f + 3.14159f) * 1.5f;
    
    // Luz 4: Movimento circular em fase oposta à luz 2
    m_lightPositions[3].x = sin(time + 3.14159f) * radius;
    m_lightPositions[3].z = cos(time + 3.14159f) * radius;
    
    // Atualizar cores das luzes
    // Luz 1: Transição de vermelho para amarelo
    m_lightColors[0] = glm::vec3(
        1.0f,
        0.5f + 0.5f * sin(time * 0.7f),
        0.0f
    );
    
    // Luz 2: Transição de azul para ciano
    m_lightColors[1] = glm::vec3(
        0.0f,
        0.5f + 0.5f * sin(time * 0.9f),
        1.0f
    );
    
    // Luz 3: Transição de verde para amarelo
    m_lightColors[2] = glm::vec3(
        0.5f + 0.5f * sin(time * 0.8f),
        1.0f,
        0.0f
    );
    
    // Luz 4: Transição de roxo para rosa
    m_lightColors[3] = glm::vec3(
        0.8f,
        0.0f,
        0.5f + 0.5f * sin(time * 1.1f)
    );
}

void Renderer::setLightColor(int lightIndex, const glm::vec3& color)
{
    if (lightIndex >= 0 && lightIndex < MAX_LIGHTS)
    {
        m_lightColors[lightIndex] = color;
    }
}

void Renderer::setLightPosition(int lightIndex, const glm::vec3& position)
{
    if (lightIndex >= 0 && lightIndex < MAX_LIGHTS)
    {
        m_lightPositions[lightIndex] = position;
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
    // Configurar posições iniciais das luzes
    m_lightPositions[0] = glm::vec3(0.0f, 2.0f, 0.0f);    // Topo
    m_lightPositions[1] = glm::vec3(2.0f, 0.0f, 0.0f);    // Direita
    m_lightPositions[2] = glm::vec3(0.0f, -2.0f, 0.0f);   // Base
    m_lightPositions[3] = glm::vec3(-2.0f, 0.0f, 0.0f);   // Esquerda
    
    // Configurar cores iniciais das luzes
    m_lightColors[0] = glm::vec3(1.0f, 0.5f, 0.0f);       // Laranja
    m_lightColors[1] = glm::vec3(0.0f, 0.5f, 1.0f);       // Azul claro
    m_lightColors[2] = glm::vec3(0.5f, 1.0f, 0.0f);       // Verde claro
    m_lightColors[3] = glm::vec3(0.8f, 0.0f, 0.5f);       // Roxo
}
