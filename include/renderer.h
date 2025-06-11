#ifndef RENDERER_H
#define RENDERER_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX  // Evita conflitos com min/max do Windows
    #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
#endif

#include <glad/glad.h>
#include <string>
#include <vector>
#include "camera.h"
#include "Object/Meshes/Mesh.h"
#include "window.h"

class SceneObject;
/**
 * @class Renderer
 * @brief Classe para gerenciamento do pipeline de renderização
 */
class Renderer {
public:
    /**
     * @brief Construtor
     * @param window Referência para a janela
     */
    explicit Renderer(Window& window);
    ~Renderer();
    
    /**
     * @brief Inicializa o renderer
     * @return true se a inicialização foi bem-sucedida, false caso contrário
     */
    bool initialize();
    
    /**
     * @brief Renderiza um frame
     * @param camera Câmera utilizada para renderização
     * @param mesh Cubo a ser renderizado
     * @param deltaTime Tempo desde o último frame
     * @param currentFrame Número do frame atual
     */
    void renderFrame(const Camera& camera, SceneObject& object, float deltaTime);
    
    /**
     * @brief Salva o frame atual como imagem
     * @param outputPath Caminho para salvar a imagem
     * @param frameNumber Número do frame
     * @return true se o salvamento foi bem-sucedido, false caso contrário
     */
    bool saveFrameToImage(const std::string& outputPath, int frameNumber);
    
    /**
     * @brief Define a cor da luz
     * @param lightIndex Índice da luz (0-3)
     * @param color Cor da luz (RGB)
     */
    void setLightColor(int lightIndex, const glm::vec3& color);
    
    /**
     * @brief Define a posição da luz
     * @param lightIndex Índice da luz (0-3)
     * @param position Posição da luz
     */
    void setLightPosition(int lightIndex, const glm::vec3& position);
    
    /**
     * @brief Combina frames em um vídeo
     * @param framesPath Caminho para os frames
     * @param outputPath Caminho para o vídeo de saída
     * @param frameCount Número total de frames
     * @param fps Frames por segundo
     * @return true se a combinação foi bem-sucedida, false caso contrário
     */
    bool combineFramesToVideo(const std::string& framesPath, const std::string& outputPath, 
                             int frameCount, int fps);

private:
    /** @brief Configura as luzes iniciais*/
    void setupLights();
    void updateLights(float deltaTime);
    
    Window& m_window;

    float m_accumulateTime = 0.0f;

    std::vector<Light> m_lights;
    
};
#endif