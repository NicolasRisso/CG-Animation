#include "optimization.h"
#include "renderer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <filesystem>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "Light.h"
#include "Object/SceneObject.h"
#include "Utility/Constants/EngineLimits.h"

// Implementação otimizada do Renderer

extern std::queue<std::pair<std::shared_ptr<std::vector<unsigned char>>, std::string>> g_saveQueue;
extern std::mutex                                     g_queueMutex;
extern std::condition_variable                        g_queueCV;

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

void Renderer::renderFrame(const Camera& camera, const Scene& scene, const float deltaTime)
{
    // Limpar buffer
    m_window.clear(0.05f, 0.05f, 0.05f, 1.0f);
    
    // Atualizar posições das luzes
    updateLights(deltaTime);

    const std::vector<SceneObject*> sceneObjects = scene.GetObjectsFromScene();
    for (const SceneObject* object : sceneObjects)
        object->Draw(camera.getViewMatrix(), camera.getProjectionMatrix(m_window.getAspectRatio()), camera.getPosition(), m_lights);
    
    m_window.update();
}

bool Renderer::saveFrameToImage(const std::string& outputPath, const int frameNumber)
{
    int width  = m_window.getWidth();
    int height = m_window.getHeight();
    std::vector<unsigned char> pixels(width * height * 4);

    glReadPixels(0, 0, width, height,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // o stb_image_write escreve origin (0,0) em top-left se ativarmos:
    stbi_flip_vertically_on_write(true);

    std::ostringstream ss;
    ss << outputPath
       << "/frame_" << std::setw(5) << std::setfill('0')
       << frameNumber << ".png";
    std::string filename = ss.str();

    // salva como PNG RGBA8
    if (!stbi_write_png(filename.c_str(),
                        width, height, 4,
                        pixels.data(),
                        width * 4))
    {
        std::cerr << "Erro ao salvar frame em " << filename << "\n";
        return false;
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
