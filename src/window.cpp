#include "window.h"
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr)
{
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

bool Window::initialize(bool bShouldUseVsync)
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return false;
    }
    
    // Configurar GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Criar janela
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // Tornar o contexto OpenGL atual
    glfwMakeContextCurrent(m_window);
    
    // Configurar callback de redimensionamento
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetWindowUserPointer(m_window, this);

    // VSync
    glfwSwapInterval(bShouldUseVsync ? 1 : 0);
    
    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return false;
    }
    
    // Configurar viewport
    glViewport(0, 0, m_width, m_height);
    
    return true;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::getGLFWWindow() const
{
    return m_window;
}

void Window::setFramebufferSizeCallback(std::function<void(int, int)> callback)
{
    // Implementação será adicionada quando necessário
}

int Window::getWidth() const
{
    return m_width;
}

int Window::getHeight() const
{
    return m_height;
}

float Window::getAspectRatio() const
{
    return static_cast<float>(m_width) / static_cast<float>(m_height);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    // Atualizar dimensões da janela
    Window* windowObj = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowObj)
    {
        windowObj->m_width = width;
        windowObj->m_height = height;
    }
}

void Window::processInput(float deltaTime)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_window, true);
}