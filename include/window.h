#ifndef WINDOW_H
#define WINDOW_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX  // Evita conflitos com min/max do Windows
    #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #include <windows.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

/**
 * @class Window
 * @brief Classe para gerenciamento de janela GLFW
 */
class Window {
public:
    /**
     * @brief Construtor da janela
     * @param width Largura da janela
     * @param height Altura da janela
     * @param title Título da janela
     */
    Window(int width, int height, const std::string& title);
    
    /**
     * @brief Destrutor da janela
     */
    ~Window();
    
    /**
     * @brief Inicializa a janela e contexto OpenGL
     * @return true se a inicialização foi bem-sucedida, false caso contrário
     */
    bool initialize(bool bShouldUseVsync = true);
    
    /**
     * @brief Verifica se a janela deve ser fechada
     * @return true se a janela deve ser fechada, false caso contrário
     */
    bool shouldClose() const;
    
    /**
     * @brief Troca os buffers e processa eventos
     */
    void update();
    
    /**
     * @brief Limpa a tela com a cor especificada
     * @param r Componente vermelho (0.0f - 1.0f)
     * @param g Componente verde (0.0f - 1.0f)
     * @param b Componente azul (0.0f - 1.0f)
     * @param a Componente alfa (0.0f - 1.0f)
     */
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    
    /**
     * @brief Obtém o ponteiro para a janela GLFW
     * @return Ponteiro para a janela GLFW
     */
    GLFWwindow* getGLFWWindow() const;
    
    /**
     * @brief Define a função de callback para redimensionamento da janela
     * @param callback Função de callback
     */
    void setFramebufferSizeCallback(std::function<void(int, int)> callback);
    
    /**
     * @brief Obtém a largura atual da janela
     * @return Largura da janela
     */
    int getWidth() const;
    
    /**
     * @brief Obtém a altura atual da janela
     * @return Altura da janela
     */
    int getHeight() const;
    
    /**
     * @brief Obtém a razão de aspecto da janela
     * @return Razão de aspecto (largura/altura)
     */
    float getAspectRatio() const;

    /**
    * @brief Processa o Input recebido em DeltaTime 
    */
    void processInput(float deltaTime);
    
private:
    int m_width;                 ///< Largura da janela
    int m_height;                ///< Altura da janela
    std::string m_title;         ///< Título da janela
    GLFWwindow* m_window;        ///< Ponteiro para a janela GLFW
    
    /**
     * @brief Callback estático para redimensionamento da janela
     * @param window Ponteiro para a janela GLFW
     * @param width Nova largura
     * @param height Nova altura
     */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif // WINDOW_H
